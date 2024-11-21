package main

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"strconv"
	"strings"
)

type Game struct {
	Name              string  `json:"name"`
	MinimumCPU        string  `json:"minimumCpu"`
	MinimumCPUScore   int     `json:"minimumCpuScore"`
	MinimumGPU        string  `json:"minimumGpu"`
	MinimumGPUScore   int     `json:"minimumGpuScore"`
	MinimumMem        float64 `json:"minimumMem"`
	RecommendCPU      string  `json:"recommendCpu"`
	RecommendCPUScore int     `json:"recommendCpuScore"`
	RecommendGPU      string  `json:"recommendGpu"`
	RecommendGPUScore int     `json:"recommendGpuScore"`
	RecommendMem      float64 `json:"recommendMem"`
}

type GameFinder struct {
	Games []Game
	// apiSession      string
	apiGetCpuName   string
	apiGetGpuName   string
	apiGetMedianUrl string
}

type Cpu3dMark struct {
	Id    string `json:"id"`
	Label string `json:"label"`
	Value string `json:"value"`
}

type Gpu3dMark struct {
	Id    string `json:"id"`
	Label string `json:"label"`
	Value string `json:"value"`
}

func NewGameFinder() GameFinder {
	str, _ := os.ReadFile("./games.json")
	var games []Game
	json.Unmarshal(str, &games)
	var gameFinder GameFinder
	gameFinder.Games = games
	gameFinder.apiGetCpuName = "https://www.3dmark.com/proxycon/ajax/search/cpuname?term=%s"
	gameFinder.apiGetGpuName = "https://www.3dmark.com/proxycon/ajax/search/gpuname?term=%s"
	gameFinder.apiGetMedianUrl = "https://www.3dmark.com/proxycon/ajax/medianscore?test=fs%%20P&cpuId=%s&gpuId=%s&gpuCount=1&gpuType=ALL&deviceType=ALL&storageModel=ALL&showRamDisks=false&memoryChannels=0&country=&scoreType=%s&hofMode=false&showInvalidResults=false&freeParams=&minGpuCoreClock=&maxGpuCoreClock=&minGpuMemClock=&maxGpuMemClock=&minCpuClock=&maxCpuClock="
	// gameFinder.apiSession = getSession()
	return gameFinder
}

// func getSession() string { //Session을 주지 않으면 proxycon의 median부분이 동작하지 않음(406 Not Acceptable)
// 	url := "https://www.3dmark.com/search"
// 	result, _ := http.Get(url)
// 	for _, value := range result.Cookies() {
// 		if value.Name == "FM_PROXYCON_SESSION" {
// 			fmt.Println(value.Value)
// 			return value.Value
// 		}
// 	}
// 	return ""
// }

func (gameFinder *GameFinder) GetGameList() string {
	str := ""
	for _, game := range gameFinder.Games {
		str = str + game.Name + "\n"
	}
	str = str[:len(str)-1]
	return str
}

func (gameFinder *GameFinder) GetGame(name string) Game {
	for _, game := range gameFinder.Games {
		if game.Name == name {
			return game
		}
	}
	return Game{}
}
func (gameFinder *GameFinder) CheckCpuRun(cpuScore float64, game Game) int { //0: Not, 1: Bigger than minimum, 2: Yes
	if cpuScore < float64(game.MinimumCPUScore) {
		return 0
	} else if cpuScore < float64(game.RecommendCPUScore) {
		return 1
	}
	return 2
}

func (gameFinder *GameFinder) GetCpuScore(cpuName string) float64 {
	cpuId := strconv.Itoa(gameFinder.getCpuId(cpuName))
	url := fmt.Sprintf(gameFinder.apiGetMedianUrl, cpuId, "", "physicsScore")
	fmt.Println(url)
	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Add("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:132.0) Gecko/20100101 Firefox/132.0")
	// sessionCookie := http.Cookie{
	// 	Name:  "FM_PROXYCON_SESSION",
	// 	Value: gameFinder.apiSession,
	// }
	// req.Header.Add("Cookie", sessionCookie.String())
	req.Header.Add("Accept", "application/json, text/javascript, */*; q=0.01")
	client := &http.Client{}
	result, _ := client.Do(req)
	// result, _ := http.Get(url)
	var data map[string]float64

	defer result.Body.Close()
	str, _ := io.ReadAll(result.Body)
	fmt.Printf("%s\n", str)
	json.Unmarshal(str, &data)
	for key, value := range data {
		fmt.Println(key, value)
	}
	return data["median"]
}

func (gameFinder *GameFinder) getCpuId(cpuName string) int {
	url := fmt.Sprintf(gameFinder.apiGetCpuName, cpuName)
	url = strings.ReplaceAll(url, " ", "%20")
	fmt.Println(url)
	result, _ := http.Get(url)
	var data []Cpu3dMark
	str, _ := io.ReadAll(result.Body)
	fmt.Printf("%s\n", str)
	json.Unmarshal(str, &data)
	for _, value := range data {
		if value.Label == cpuName {
			id, _ := strconv.Atoi(value.Id)
			return id
		}
	}
	id, _ := strconv.Atoi(data[0].Id)
	return id
}

func (gameFinder *GameFinder) CheckGpuRun(gpuScore float64, game Game) int {
	if gpuScore < float64(game.MinimumGPUScore) {
		return 0
	} else if gpuScore < float64(game.RecommendGPUScore) {
		return 1
	}
	return 2
}
func (gameFinder *GameFinder) GetGpuScore(gpuName string) float64 {
	gpuId := strconv.Itoa(gameFinder.getGpuId(gpuName))
	url := fmt.Sprintf(gameFinder.apiGetMedianUrl, "", gpuId, "graphicsScore")
	fmt.Println(url)
	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Add("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:132.0) Gecko/20100101 Firefox/132.0")
	req.Header.Add("Accept", "application/json, text/javascript, */*; q=0.01")
	client := &http.Client{}
	result, _ := client.Do(req)
	// result, _ := http.Get(url)
	var data map[string]float64

	defer result.Body.Close()
	str, _ := io.ReadAll(result.Body)
	fmt.Printf("%s\n", str)
	json.Unmarshal(str, &data)
	for key, value := range data {
		fmt.Println(key, value)
	}
	return data["median"]
}
func (gameFinder *GameFinder) getGpuId(gpuName string) int {
	url := fmt.Sprintf(gameFinder.apiGetGpuName, gpuName)
	url = strings.ReplaceAll(url, " ", "%20")
	fmt.Println(url)
	result, _ := http.Get(url)
	var data []Cpu3dMark
	str, _ := io.ReadAll(result.Body)
	fmt.Printf("%s\n", str)
	json.Unmarshal(str, &data)
	for _, value := range data {
		if value.Label == gpuName {
			id, _ := strconv.Atoi(value.Id)
			return id
		}
	}
	id, _ := strconv.Atoi(data[0].Id)
	return id
}

func (gameFinder *GameFinder) CheckMemRun(memSize float64, game Game) int {
	if memSize < game.MinimumMem {
		return 0
	} else if memSize < game.RecommendMem {
		return 1
	}
	return 2
}

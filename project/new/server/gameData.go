package main

import (
	"encoding/json"
	"fmt"
	"os"
)

type Game struct {
	Name              string `json: "name"`
	MinimumCPU        string `json: "minimumCpu"`
	MinimumCPUScore   int    `json: "minimumCpuScore"`
	MinimumGPU        string `json: "minimumGpu"`
	MinimumGPUScore   int    `json: "minimumGpuScore"`
	RecommendCPU      string `json: "recommendCpu"`
	RecommendCPUScore int    `json: "recommendCpuScore"`
	RecommendGPU      string `json: "recommendGpu"`
	RecommendGPUScore int    `json: "recommendGpuScore"`
}

type GameFinder struct {
	Games []Game
}

func NewGameFinder() GameFinder {
	str, _ := os.ReadFile("./games.json")
	var games []Game
	json.Unmarshal(str, &games)
	fmt.Printf("Unmarshal: %v", games)
	return GameFinder{games}
}

func (gameFinder *GameFinder) GetGameList() string {
	str := ""
	for _, game := range gameFinder.Games {
		str = str + game.Name + "\n"
	}
	str = str[:len(str)-1]
	return str
}

func (GameFinder *GameFinder) CheckCpuRun(cpuScore int, gameIndex int) int { //0: Not, 1: Bigger than minimum, 2: Yes
	return 0
}

func (gameFinder *GameFinder) GetCpuScore(cpuName string) int {
	return 0
}

func (gameFinder *GameFinder) CheckGpuRun(gpuScore int, gameIndex int) int {
	return 0
}
func (gameFinder *GameFinder) GetGpuScore(gpuName string, gameIndex int) int {
	return 0
}

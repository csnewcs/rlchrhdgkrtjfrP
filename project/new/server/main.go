package main

import (
	"fmt"
	"strconv"

	"github.com/labstack/echo/v4"
)

var gameFinder GameFinder

func main() {
	gameFinder = NewGameFinder()
	server := echo.New()
	server.GET("/", func(c echo.Context) error {
		return c.String(200, "hi")
	})
	server.GET("/gamelist", func(c echo.Context) error {
		return c.String(200, gameFinder.GetGameList())
	})
	server.GET("/cpucheck/:game", checkCpu)
	server.GET("/memcheck/:game", checkMem)
	server.GET("/gpucheck/:game", checkGpu)
	server.Logger.Fatal(server.Start(":8080"))
}

func getGame(gameId string) (Game, error) {
	id, err := strconv.Atoi(gameId)
	if id >= len(gameFinder.Games) || id < 0 || err != nil {
		return Game{}, fmt.Errorf("invalid game id")
	}
	return gameFinder.Games[id], nil
}

func checkCpu(c echo.Context) error {
	name := c.QueryParam("cpu")
	gameId := c.Param("game")
	if name == "" {
		return c.String(400, "-1\nNo cpu name")
	}

	game, err := getGame(gameId)
	if err != nil {
		return c.String(400, "-1\nInvalid game id")
	}
	cpuScore := gameFinder.GetCpuScore(name)
	result := gameFinder.CheckCpuRun(cpuScore, game)
	return c.String(200, fmt.Sprintf("%d\n%s\n%s,%s,%dGB\n%s,%s,%dGB", result, name, game.MinimumCPU, game.MinimumGPU, game.MinimumMem, game.RecommendCPU, game.RecommendGPU, game.RecommendMem))
	//첫째 줄: 실행 가능 여부(0: 불가능, 1: 최소 사양 이상, 2: 권장 사양 이상)
	//둘째 줄: Request의 성능(CPU)
	//셋째 줄: 최소 사양
	//넷째 줄: 권장 사양
}
func checkMem(c echo.Context) error { //GB 단위로 받음
	memory := c.QueryParam("mem")
	gameId := c.Param("game")
	memorySize, err := strconv.Atoi(memory)
	if err != nil {
		return c.String(400, "-1\nNo memory size")
	}

	game, err := getGame(gameId)
	if err != nil {
		return c.String(400, "-1\nInvalid game id")
	}
	result := gameFinder.CheckMemRun(memorySize, game)
	return c.String(200, fmt.Sprintf("%d\n%s,%s,%dGB\n%s,%s,%dGB", result, game.MinimumCPU, game.MinimumGPU, game.MinimumMem, game.RecommendCPU, game.RecommendGPU, game.RecommendMem))
}
func checkGpu(c echo.Context) error {
	name := c.QueryParam("gpu")
	gameId := c.Param("game")
	if name == "" {
		return c.String(400, "-1\nNo gpu name")
	}
	game, err := getGame(gameId)
	if err != nil {
		return c.String(400, "-1\nInvalid game id")
	}
	gpuScore := gameFinder.GetGpuScore(name)
	result := gameFinder.CheckGpuRun(gpuScore, game)
	return c.String(200, fmt.Sprintf("%d\n%s,%s,%dGB\n%s,%s,%dGB", result, game.MinimumCPU, game.MinimumGPU, game.MinimumMem, game.RecommendCPU, game.RecommendGPU, game.RecommendMem))
	return c.String(200, "준비 중...")
}

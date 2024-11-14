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
	server.Logger.Fatal(server.Start(":8080"))
}

func checkCpu(c echo.Context) error {
	name := c.QueryParam("cpu")
	gameId := c.Param("game")
	if name == "" {
		return c.String(400, "-1\nNo cpu name")
	}
	id, err := strconv.Atoi(gameId)
	if err != nil || id >= len(gameFinder.Games) || id < 0 {
		return c.String(400, "-1\nInvalid game id")
	}
	cpuScore := gameFinder.GetCpuScore(name)
	game := gameFinder.Games[id]
	result := gameFinder.CheckCpuRun(cpuScore, game)
	return c.String(200, fmt.Sprintf("%d\n%s\n%s,%s,%dGB\n%s,%s,%dGB", result, name, game.MinimumCPU, game.MinimumGPU, game.MinimumMem, game.RecommendCPU, game.RecommendGPU, game.RecommendMem))
	//첫째 줄: 실행 가능 여부(0: 불가능, 1: 최소 사양 이상, 2: 권장 사양 이상)
	//둘째 줄: Request의 성능(CPU)
	//셋째 줄: 최소 사양
	//넷째 줄: 권장 사양
}

package main

import (
	"fmt"
	"strconv"

	"github.com/labstack/echo/v4"
)

var gameFinder GameFinder

func main() {
	gameFinder = NewGameFinder()
	fmt.Println(gameFinder.GetCpuScore("i7-7700HQ"))
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
		return c.String(400, "-1, No cpu name")
	}
	cpuScore := gameFinder.GetCpuScore(name)
	id, err := strconv.Atoi(gameId)
	game := gameFinder.Games[id]
	if err != nil {
		return c.String(400, "-1, Invalid game id")
	}
	result := gameFinder.CheckCpuRun(cpuScore, game)
	return c.String(200, fmt.Sprintf("%d %s %s %s %dGB %s %s %dGB", result, name, game.MinimumCPU, game.MinimumGPU, game.MinimumMem, game.RecommendCPU, game.RecommendGPU, game.RecommendMem))
}

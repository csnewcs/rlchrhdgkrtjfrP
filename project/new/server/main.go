package main

import "github.com/labstack/echo/v4"

func main() {
	gameFinder := NewGameFinder()
	server := echo.New()
	server.GET("/", func(c echo.Context) error {
		return c.String(200, "hi")
	})
	server.GET("/gamelist", func(c echo.Context) error {
		return c.String(200, gameFinder.GetGameList())
	})
	server.Logger.Fatal(server.Start(":8080"))
}

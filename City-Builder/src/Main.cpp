#include "Game.hpp"
#include "Game-State-Start.hpp"

int main()
{
	Game game;

	game.pushState(new GameStateStart(&game));
	game.gameLoop();

	return 0;
}

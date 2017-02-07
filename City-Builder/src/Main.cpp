#include "Game.hpp"
#include "Game-State-Start.hpp"
#include <iostream>

int main()
{
	Game game;

	game.pushState(new GameStateStart(&game));
	std::cout << "Press space to load game, esc to exit\n";
	game.gameLoop();

	return 0;
}

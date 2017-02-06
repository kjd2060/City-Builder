#pragma once

#include <stack>
#include <SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class GameState;

class Game
{
public:
	std::stack<GameState*> states;
	SDL_Window* window;
	SDL_Surface* screenSurface;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();
	
	void InitSDL();
	void gameLoop();

	Game();
	~Game();
};
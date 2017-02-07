#pragma once

#include <SFML/Graphics.hpp>

#include "Game-State.hpp"

class GameStateStart : public GameState
{
private:

	sf::View view;

	void loadgame();

public:

	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateStart(Game* game);
};
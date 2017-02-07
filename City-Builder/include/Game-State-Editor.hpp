#pragma once

#include <SFML/Graphics.hpp>

#include "Game-State.hpp"
#include "World.hpp"

enum class ActionState { NONE, PANNING };

class GameStateEditor : public GameState
{
private:

	ActionState actionState;

	sf::View gameView;
	sf::View guiView;

	World world;

	sf::Vector2i panningAnchor;
	float zoomLevel;

public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateEditor(Game* game);
};
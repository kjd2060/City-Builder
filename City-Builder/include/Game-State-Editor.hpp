#pragma once

#include <SFML/Graphics.hpp>

#include "Game-State.hpp"
#include "World.hpp"

enum class ActionState { NONE, PANNING, ZOOMING };

class GameStateEditor : public GameState
{
private:

	ActionState actionState;

	sf::View gameView;
	sf::View guiView;

	World world;

	sf::Vector2i panningAnchor;
	float zoomLevel;
	float previousZoom;

public:

	const float zoomMax = 3.25;
	const float zoomMin = .25;

	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateEditor(Game* game);
};
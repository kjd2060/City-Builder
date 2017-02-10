#pragma once

#include <SFML/Graphics.hpp>

#include "Game-State.hpp"
#include "World.hpp"
#include "City.hpp"

enum class ActionState { NONE, PANNING, ZOOMING, SELECTING };

class GameStateEditor : public GameState
{
private:

	ActionState actionState;

	sf::View gameView;
	sf::View guiView;

	City city;

	sf::Vector2i panningAnchor;
	float zoomLevel;
	float previousZoom;

	sf::Vector2i selectionStart;
	sf::Vector2i selectionEnd;

	Tile* currentTile;

	std::map<std::string, Gui> guiSystem;

	void zoom(sf::Event &event);

public:

	const float zoomMax = 3.25;
	const float zoomMin = .25;

	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateEditor(Game* game);
};
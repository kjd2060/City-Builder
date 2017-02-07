#include <SFML/Graphics.hpp>

#include "Game-State.hpp"
#include "Game-State-Editor.hpp"

void GameStateEditor::draw(const float dt)
{
	this->game->window.clear(sf::Color::Black);
	
	this->game->window.setView(this->guiView);
	this->game->window.draw(this->game->background);

	this->game->window.setView(this->gameView);
	this->world.draw(this->game->window, dt);
	
	return;
}

void GameStateEditor::update(const float dt)
{
	return;
}

void GameStateEditor::handleInput()
{
	sf::Event event;

	while (this->game->window.pollEvent(event))
	{
		switch (event.type)
		{
		// close window
		case sf::Event::Closed:
		{
			this->game->window.close();
			break;
		}
		// resize window
		case sf::Event::Resized:
		{
			gameView.setSize(event.size.width, event.size.height);
			guiView.setSize(event.size.width, event.size.height);
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->guiView));
			this->game->background.scale(
				float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}
		default:break;
		}
	}

	return;
}

GameStateEditor::GameStateEditor(Game* game)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->guiView.setSize(pos);
	this->gameView.setSize(pos);
	pos *= 0.5f;
	this->guiView.setCenter(pos);
	this->gameView.setCenter(pos);

	world = World("maps/city-map01.dat", 64, 64, game->tileAtlas);

	this->zoomLevel = 1.0f;

	// center camera on map
	sf::Vector2f center(this->world.width, this->world.height*0.5);
	center *= float(this->world.tileSize);
	gameView.setCenter(center);

	this->actionState = ActionState::NONE;
}
#include <SFML/Graphics.hpp>

#include "Game-State.hpp"
#include "Game-State-Editor.hpp"
#include <iostream>
#include "KMath.hpp"

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
			gameView.zoom(zoomLevel);
			guiView.setSize(event.size.width, event.size.height);
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->guiView));
			this->game->background.scale(
				float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}
		case sf::Event::MouseMoved:
		{
			// pan the camera
			if (this->actionState == ActionState::PANNING)
			{
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
				gameView.move(-1.0f * pos * this->zoomLevel);
				panningAnchor = sf::Mouse::getPosition(this->game->window);
			}

			// select tiles
			else if (actionState == ActionState::SELECTING)
			{
				sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
				selectionEnd.x = pos.y / (this->world.tileSize) + pos.x / (2 * this->world.tileSize) - this->world.width * 0.5 - .5;
				selectionEnd.y = pos.y / (this->world.tileSize) - pos.x / (2 * this->world.tileSize) + this->world.width * 0.5 + .5;

				this->world.clearSelected();

				if (this->currentTile->tileType == TileType::GRASS)
				{
					this->world.select(selectionStart, selectionEnd, { this->currentTile->tileType, TileType::WATER });
				}
				else
				{
					this->world.select(selectionStart, selectionEnd,
					{
						this->currentTile->tileType, TileType::FOREST,
						TileType::WATER,			 TileType::ROAD,
						TileType::RESIDENTIAL,		 TileType::COMMERCIAL,
						TileType::INDUSTRIAL
					});
				}
			}
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			// start panning
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				if (this->actionState != ActionState::PANNING)
				{
					this->actionState = ActionState::PANNING;
					this->panningAnchor = sf::Mouse::getPosition(this->game->window);
				}
			}

			// select map tile
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->actionState != ActionState::SELECTING)
				{
					this->actionState = ActionState::SELECTING;
					sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
					selectionStart.x = pos.y / (this->world.tileSize) + pos.x / (2 * this->world.tileSize) - this->world.width * 0.5 - 0.5;
					selectionStart.y = pos.y / (this->world.tileSize) - pos.x / (2 * this->world.tileSize) + this->world.width * 0.5 + 0.5;
				}
			}

			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				// stop selecting
				if (this->actionState == ActionState::SELECTING)
				{
					this->actionState = ActionState::NONE;
					this->world.clearSelected();
				}
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			// stop panning
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				this->actionState = ActionState::NONE;
			}

			// stop selecting
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->actionState == ActionState::SELECTING)
				{
					this->actionState = ActionState::NONE;
					this->world.clearSelected();
				}
			}
			break;
		}
		case sf::Event::MouseWheelMoved:
		{
			// handle zooming in seperate function; zooms dynamically using a LERP
			zoom(event);
		}

		break;
		default:break;
		}
	}

	return;
}

void GameStateEditor::zoom(sf::Event &event)
{
	this->actionState = ActionState::ZOOMING;
	float targetZoom = 1.0f;
	if (event.mouseWheel.delta < 0 && zoomLevel <= zoomMax)
		targetZoom = 2.25f;
	else if (zoomLevel >= zoomMin && event.mouseWheel.delta > 0)
		targetZoom = 0.25f;
	else
		return;

	float currentZoom = 1.0f;
	previousZoom = currentZoom;


	sf::Vector2f currentCenter = gameView.getCenter();
	sf::Vector2f targetCenter = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), gameView);


	float lerpFactor = 0.15f;
	currentZoom = KMath::lerp(lerpFactor, currentZoom, targetZoom);
	gameView.zoom(1.0f + (currentZoom - previousZoom));

	currentCenter.x = KMath::lerp(lerpFactor, currentCenter.x, targetCenter.x);
	currentCenter.y = KMath::lerp(lerpFactor, currentCenter.y, targetCenter.y);
	gameView.setCenter(currentCenter);

	zoomLevel *= 1.0f + (currentZoom - previousZoom);

	previousZoom = currentZoom;

	if (fabs(targetZoom - currentZoom) < 0.01f)
		this->actionState = ActionState::NONE;

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

	this->selectionStart = sf::Vector2i(0, 0);
	this->selectionEnd = sf::Vector2i(0, 0);

	this->currentTile = &this->game->tileAtlas.at("grass");

	world.view = &gameView;
}
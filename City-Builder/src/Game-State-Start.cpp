#include <SFML/Graphics.hpp>

#include <utility>

#include "Game-State-Start.hpp"
#include "Game-State-Editor.hpp"
#include "Game-State.hpp"

void GameStateStart::draw(const float dt)
{
	this->game->window.setView(this->view);

	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);

	for (auto gui : this->guiSystem)
		this->game->window.draw(gui.second);

	return;
}

void GameStateStart::update(const float dt)
{

}

void GameStateStart::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);


	while (this->game->window.pollEvent(event))
	{
		switch (event.type)
		{
			// if its window clsoe event, close the window
		case sf::Event::Closed:
			game->window.close();
			break;
		// resize window
		case sf::Event::Resized:
		{
			this->view.setSize(event.size.width, event.size.height);
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));

			this->game->background.setScale(
				float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}

		// highlight menu items
		case sf::Event::MouseMoved:
		{
			this->guiSystem.at("menu").highlight(this->guiSystem.at("menu").getEntry(mousePos));
			break;
		}

		// click on menu items
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::string msg = this->guiSystem.at("menu").activate(mousePos);

				if (msg == "load_game")
				{
					this->loadgame();
				}
			}
			break;
		}

		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape)
				this->game->window.close();
			break;
		}
		default:break;
		}
	}

	return;
}

void GameStateStart::loadgame()
{
	this->game->pushState(new GameStateEditor(this->game));

	return;
}

GameStateStart::GameStateStart(Game* game)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->view.setSize(pos);
	pos *= 0.5f;
	this->view.setCenter(pos);

	this->guiSystem.emplace("menu", Gui(sf::Vector2f(192, 32), 4, false, game->stylesheets.at("button"),
	{ std::make_pair("Load Game", "load_game") }));

	this->guiSystem.at("menu").setPosition(pos);
	this->guiSystem.at("menu").setOrigin(96, 32 * 0.5);
	this->guiSystem.at("menu").show();

}
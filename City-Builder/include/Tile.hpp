#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "AnimationHandler.hpp"

enum class TileType { VOID, GRASS, FOREST, WATER, RESIDENTIAL, COMMERCIAL, INDUSTRIAL, ROAD };

std::string tileTypeToStr(TileType type);

class Tile
{
public:

	AnimationHandler animHandler;
	sf::Sprite sprite;

	TileType tileType;

	// tile variant, allowing for different looking versions of same tile
	int tileVariant;

	// region IDs of the tile, tiles in same region are connected.  First is for transport
	unsigned int regions[1];

	// placement cost of tile
	unsigned int cost;

	// current residents / employees
	double population;
	// max pop growth per stage / tile variant
	unsigned int maxPopPerLevel;
	// max num of building levels
	unsigned int maxLevels;
	// production output per customer/worker per day, either monetary or goods
	float production;
	// goods stored
	float storedGoods;

	// constructor
	Tile() {}
	Tile(const unsigned int tileSize, const unsigned int height, sf::Texture &texture,
		const std::vector<Animation> &animations,
		const TileType tileType, const unsigned int cost, const unsigned int maxPopPerLevel,
		const unsigned int maxLevels)
	{
		this->tileType = tileType;
		this-> tileVariant = 0;
		this->regions[0] = 0;

		this->cost = cost;
		this->population = 0;
		this->maxPopPerLevel = maxPopPerLevel;
		this->maxLevels = maxLevels;
		this->production = 0;
		this->storedGoods = 0;

		this->sprite.setOrigin(sf::Vector2f(0.0f, tileSize*(height - 1)));
		this->sprite.setTexture(texture);
		this->animHandler.frameSize = sf::IntRect(0, 0, tileSize * 2, tileSize*height);
		for (auto animation : animations)
		{
			this->animHandler.addAnim(animation);
		}
		this->animHandler.update(0.0f);
	}

	void draw(sf::RenderWindow &window, float dt);

	void update();

	// return string containing display cost of tile */
	std::string getCost()
	{
		return std::to_string(this->cost);
	}
};
#include <SFML/Graphics.hpp>

#include "AnimationHandler.hpp"
#include "Tile.hpp"

void Tile::draw(sf::RenderWindow &window, float dt)
{
	// change sprite to reflect the tile variant
	this->animHandler.changeAnim(this->tileVariant);

	// update animation
	this->animHandler.update(dt);

	// update sprite
	this->sprite.setTextureRect(this->animHandler.bounds);

	// draw tile
	window.draw(this->sprite);

	return;
}

void Tile::update()
{
	/*
	*  If population is at maximum for this tile,
	*  there is a chance the tile will increase its stage
	*/
	if ((this->tileType == TileType::RESIDENTIAL ||
		this->tileType == TileType::COMMERCIAL ||
		this->tileType == TileType::INDUSTRIAL) &&
		this->population == this->maxPopPerLevel * (this->tileVariant + 1) &&
		this->tileVariant < this->maxLevels)
	{
		if (rand() % int(1e3) < 1e2 / (this->tileVariant + 1))
			++this->tileVariant;
	}

	return;
}

std::string tileTypeToStr(TileType type)
{
	switch (type)
	{
	default:
	case TileType::VOID:		return "Void";
	case TileType::GRASS:		return "Flatten";
	case TileType::FOREST:		return "Forest";
	case TileType::WATER:		return "Water";
	case TileType::RESIDENTIAL: return "Residential Zone";
	case TileType::COMMERCIAL:  return "Commercial Zone";
	case TileType::INDUSTRIAL:  return "Industrial Zone";
	case TileType::ROAD:		return "Road";
	}
}
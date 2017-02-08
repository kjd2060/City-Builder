#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <vector>

#include <Tile.hpp>

class World
{
private:

	

	void depthfirstsearch(std::vector<TileType> &whiteList, sf::Vector2i pos, int label, int type);

public:

	unsigned int width;
	unsigned int height;

	sf::View *view;
	std::vector <Tile> tiles;

	// resource map
	std::vector<int> resources;

	// 0 = selected, 1 = selected, 2 = invalid
	std::vector<char> selected;
	unsigned int numSelected;
	
	unsigned int tileSize;
	unsigned int numRegions[1];

	// load map from disk
	void load(const std::string &filename, unsigned int width, unsigned int height,
		std::map<std::string, Tile> &tileAtlas);

	// save map to disk
	void save(const std::string &filename);

	// draw the world
	void draw(sf::RenderWindow &window, float dt);

	// checks if one position in the map is connected to another
	// by only traversing tiles in the whitelist
	void findConnectedRegions(std::vector<TileType> whitelsit, int type);

	// update direction of directional tiles so they face the right way
	// used to orient roads, pylons, rivers, etc
	void updateDirection(TileType tileType);

	void select(sf::Vector2i start, sf::Vector2i end, std::vector<TileType> blacklist);

	void clearSelected();

	// blank world constructor
	World()
	{
		this->tileSize = 8;
		this->width = 0;
		this->height = 0;
		this->numRegions[0] = 1;
		this->numSelected = 0;
		this->view = nullptr;
	}
	// load world from file constructor
	World(const std::string &filename, unsigned int width, unsigned int height,
		std::map<std::string, Tile> &tileAtlas)
	{
		this->tileSize = 8;
		this->numSelected = 0;
		this->view = nullptr;
		load(filename, width, height, tileAtlas);
	}
};
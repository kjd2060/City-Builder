#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>

#include "City.hpp"
#include "Tile.hpp"

double City::distributePool(double &pool, Tile &tile, double rate = 0.0)
{
	const static int moveRate = 4;

	unsigned int maxPop = tile.maxPopPerLevel * (tile.tileVariant + 1);

	// if room in zone, move up to 4 people from the pool into the zone
	if (pool > 0)
	{
		int moving = maxPop - tile.population;
		if (moving > moveRate)
			moving = moveRate;
		if (pool - moving < 0)
			moving = pool;
		pool -= moving;
		tile.population += moving;
	}

	// adjust tile population for births and deaths
	tile.population += tile.population * rate;

	// move population that can't be sustained by the pile into the pool
	if (tile.population > maxPop)
	{
		pool += tile.population - maxPop;
		tile.population = maxPop;
	}

	return tile.population;
}

void City::bulldoze(const Tile &tile)
{
	// replace selected tiles on the map with the tile and update pops accordingly
	for (int pos = 0; pos < this->map.width * this->map.height; ++pos)
	{
		if (this->map.selected[pos] == 1)
		{
			if (this->map.tiles[pos].tileType == TileType::RESIDENTIAL)
				this->populationPool += this->map.tiles[pos].population;
			else if (this->map.tiles[pos].tileType == TileType::COMMERCIAL)
				this->employmentPool += this->map.tiles[pos].population;
			else if (this->map.tiles[pos].tileType == TileType::INDUSTRIAL)
				this->employmentPool += this->map.tiles[pos].population;
			this->map.tiles[pos] = tile;
		}
	}

	return;
}

void City::shuffleTiles()
{
	while (this->shuffledTiles.size() < this->map.tiles.size() - 1)
	{
		this->shuffledTiles.push_back(0);
	}

	std::iota(shuffledTiles.begin(), shuffledTiles.end(), 1);
	std::random_shuffle(shuffledTiles.begin(), shuffledTiles.end());

	return;
}

void City::tileChanged()
{
	this->map.updateDirection(TileType::ROAD);
	this->map.findConnectedRegions(
	{
		TileType::ROAD, TileType::RESIDENTIAL,
		TileType::COMMERCIAL, TileType::INDUSTRIAL
	}, 0);

	return;
}

void City::load(std::string cityName, std::map<std::string, Tile> &tileAtlas)
{
	int width = 0;
	int height = 0;

	std::ifstream inputFile(cityName + "_cfg.dat", std::ios::in);

	std::string line;

	while (std::getline(inputFile, line))
	{
		std::istringstream lineStream(line);
		std::string key;
		if (std::getline(lineStream, key, '='))
		{
			std::string value;
			if (std::getline(lineStream, value))
			{
				if (key == "width")                  width = std::stoi(value);
				else if (key == "height")            height = std::stoi(value);
				else if (key == "day")               this->day = std::stoi(value);
				else if (key == "populationPool")    this->populationPool = std::stod(value);
				else if (key == "employmentPool")    this->employmentPool = std::stod(value);
				else if (key == "population")        this->population = std::stod(value);
				else if (key == "employable")        this->employable = std::stod(value);
				else if (key == "birthRate")         this->birthRate = std::stod(value);
				else if (key == "deathRate")         this->deathRate = std::stod(value);
				else if (key == "residentialTax")    this->residentialTax = std::stod(value);
				else if (key == "commercialTax")     this->commercialTax = std::stod(value);
				else if (key == "industrialTax")     this->industrialTax = std::stod(value);
				else if (key == "funds")             this->funds = std::stod(value);
				else if (key == "earnings")          this->earnings = std::stod(value);
			}
			else
			{
				std::cerr << "Error, no value for key " << key << std::endl;
			}
		}
	}

	inputFile.close();

	this->map.load(cityName + "_map.dat", width, height, tileAtlas);
	tileChanged();

	return;
}


void City::save(std::string cityName)
{
	std::ofstream outputFile(cityName + "_cfg.dat", std::ios::out);

	outputFile << "width=" << this->map.width << std::endl;
	outputFile << "height=" << this->map.height << std::endl;
	outputFile << "day=" << this->day << std::endl;
	outputFile << "populationPool=" << this->populationPool << std::endl;
	outputFile << "employmentPool=" << this->employmentPool << std::endl;
	outputFile << "population=" << this->population << std::endl;
	outputFile << "employable=" << this->employable << std::endl;
	outputFile << "birthRate=" << this->birthRate << std::endl;
	outputFile << "deathRate=" << this->deathRate << std::endl;
	outputFile << "residentialTax=" << this->residentialTax << std::endl;
	outputFile << "commercialTax=" << this->commercialTax << std::endl;
	outputFile << "industrialTax=" << this->industrialTax << std::endl;
	outputFile << "funds=" << this->funds << std::endl;
	outputFile << "earnings=" << this->earnings << std::endl;

	outputFile.close();

	this->map.save(cityName + "_map.dat");

	return;
}

void City::update(float dt)
{
	double popTotal = 0;
	double commercialRevenue = 0;
	double industrialRevenue = 0;

	// update game time
	this->currentTime += dt;
	if (this->currentTime < this->timePerDay)
		return;
	++day;
	this->currentTime = 0.0;
	if (day % 30 == 0)
	{
		this->funds += this->earnings;
		this->earnings = 0;
	}

	// run first pass of tile updates.  Mostly handles pool distributions
	for (int i = 0; i < this->map.tiles.size() - 1; ++i)
	{
		Tile &tile = this->map.tiles[this->shuffledTiles[i]];

		if (tile.tileType == TileType::RESIDENTIAL)
		{
			// redistribute the pool and increase the population total by the tile's population
			this->distributePool(this->populationPool, tile, this->birthRate - this->deathRate);
			popTotal += tile.population;
		}
		else if (tile.tileType == TileType::COMMERCIAL)
		{
			// hire people
			if (rand() % 100 < 10 * (1.0 - this->commercialTax))
				this->distributePool(this->employmentPool, tile, 0.00);
		}
		else if (tile.tileType == TileType::INDUSTRIAL)
		{
			// extract resources from the ground
			if (this->map.resources[i] > 0 && rand() % 100 < this->population)
			{
				++tile.production;
				--this->map.resources[i];
			}

			// hire people
			if (rand() % 100 < 10 * (1.0 - this->industrialTax))
				this->distributePool(this->employmentPool, tile, 0.0);
		}
		tile.update();
	}
	
	// run second pass.  Mostly for goods manufactoring
	for (int i = 0; i < this->map.tiles.size() - 1; ++i)
	{
		Tile &tile = this->map.tiles[this->shuffledTiles[i]];

		if (tile.tileType == TileType::INDUSTRIAL)
		{
			int receivedResources = 0;
			// receive resources from smaller and connected zones
			for (auto &tile2 : this->map.tiles)
			{
				if (tile2.regions[0] == tile.regions[0] && tile2.tileType == TileType::INDUSTRIAL)
				{
					if (tile2.production > 0)
					{
						++receivedResources;
						--tile2.production;
					}
					if (receivedResources >= tile.tileVariant + 1)
						break;
				}
			}

			// turn resources into goods
			tile.storedGoods += (receivedResources + tile.production)*(tile.tileVariant + 1);
		}
	}

	// run third pass.  handles good distribution
	for (int i = 0; i < this->map.tiles.size() - 1; ++i)
	{
		Tile &tile = this->map.tiles[this->shuffledTiles[i]];

		if (tile.tileType == TileType::COMMERCIAL)
		{
			int receivedGoods = 0;
			double maxCustomers = 0.0;
			for (auto &tile2 : this->map.tiles)
			{
				if (tile2.regions[0] == tile.regions[0] &&
					tile2.tileType == TileType::INDUSTRIAL &&
					tile2.storedGoods > 0)
				{
					while (tile2.storedGoods > 0 && receivedGoods != tile.tileVariant + 1)
					{
						--tile2.storedGoods;
						++receivedGoods;
						industrialRevenue += 100 * (1.0 - industrialTax);
					}
				}
				else if (tile2.regions[0] == tile.regions[0] &&
					tile2.tileType == TileType::RESIDENTIAL)
				{
					maxCustomers += tile2.population;
				}
				if (receivedGoods == tile.tileVariant + 1)
					break;
			}

			// calculate overall revenue for the tile
			tile.production = (receivedGoods*100.0 + rand() % 20) * (1.0 - this->commercialTax);

			double revenue = tile.production * maxCustomers * tile.population / 100.0;
			commercialRevenue += revenue;
		}
	}

	// adjust population pool for births and deaths
	// if adequate housing is available (< 10% homelessness)
	// quick note: The birth rate and death rate is set in the header, but it's also set in individual city configs.
	// This is an 'example city' that doesn't generate a new one every time, but uses one I've saved to a file.  To change the birth and death rates,
	// the values in maps/city_cfg.dat.  Once city generation is implemented, then the birth and death rates will continue to be local to cities but will be
	// defined by their cfg files.
	// at some point probably want a 'move in' and 'demand' factor instead of just upping the 'birth rate' to model people moving to the city from
	// outside
	if (this->getHomeless() < (popTotal + this->populationPool)*.10)
	{
		this->populationPool += this->populationPool * (this->birthRate - this->deathRate) * (((double)rand() / (RAND_MAX)) + 1); // birth rate + hidden 'move in rate'

		
	}
	else
	{
		//this->populationPool += this->populationPool * (this->birthRate*(.10/this->populationPool) - this->deathRate*this->getHomeless()*.1); // just the birth rate
		this->populationPool = this->populationPool;
	}
	popTotal += this->populationPool;

	// adjust employment pool for changing population
	float newWorkers = (popTotal - this->population) * this->propCanWork;
	newWorkers *= newWorkers < 0 ? -1 : 1;
	this->employmentPool += newWorkers;
	this->employable += newWorkers;
	if (this->employmentPool < 0)
		this->employmentPool = 0;
	if (this->employable < 0)
		this->employable = 0;

	// update city population
	this->population = popTotal;

	// calculate city income from tax
	this->earnings = (this->population - this->populationPool) * 15 * this->residentialTax;
	this->earnings += commercialRevenue * this->commercialTax;
	this->earnings += industrialRevenue * this->industrialTax;

	return;
}
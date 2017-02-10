#pragma once

#include <vector>
#include <map>

#include "World.hpp"

class City
{
private:
	float currentTime;
	float timePerDay;

	std::vector<int> shuffledTiles;

	// num of residents who are not in a residential zone
	double populationPool;

	// number of residents who are not currently employed but can work
	double employmentPool;

	// proportion of citizens who can work
	float propCanWork;

	// proportino of residents who die/give birth each day
	// esitmate death rate = 1 / (life expectancy * 360)
	// values are currently .000055 and .000023 respectively
	double birthRate;
	double deathRate;

	double distributePool(double &pool, Tile &tile, double rate);

public:

	World map;

	double population;
	double employable;
	double residentialTax;
	double commercialTax;
	double industrialTax;

	// running total of city earnings this month
	double earnings;
	double funds;

	int day;

	City()
	{
		this->birthRate = 0.00055;
		this->deathRate = 0.00023;
		this->propCanWork = 0.50;
		this->populationPool = 0;
		this->population = populationPool;
		this->employmentPool = 0;
		this->employable = employmentPool;
		this->residentialTax = 0.05;
		this->commercialTax = 0.05;
		this->industrialTax = 0.05;
		this->earnings = 0;
		this->funds = 0;
		this->currentTime = 0;
		this->timePerDay = 1.0;
		this->day = 0;
	}

	City(std::string cityName, int tileSize, std::map<std::string, Tile> &tileAtlas) : City()
	{
		this->map.tileSize = tileSize;
		load(cityName, tileAtlas);
	}

	void load(std::string cityName, std::map<std::string, Tile> &tileAtlas);
	void save(std::string cityName);

	void update(float dt);
	void bulldoze(const Tile &tile);
	void shuffleTiles();
	void tileChanged();

	double getHomeless()
	{
		return this->populationPool;
	}
	double getUnemployed()
	{
		return this->employmentPool;
	}
};
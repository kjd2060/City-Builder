#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class TextureManager
{
private:

	// array of textures used
	std::map<std::string, sf::Texture> textures;

public:

	// add texture from file
	void loadTexture(const std::string &name, const std::string &filename);

	// translate an id into a reference
	sf::Texture& getRef(const std::string &texture);

	// constructor
	TextureManager()
	{
	}

};
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#include "Texture-Manager.hpp"

void TextureManager::loadTexture(const std::string &name, const std::string &filename)
{
	// load the texture
	sf::Texture tex;
	tex.loadFromFile(filename);

	// add to list of textures
	this->textures[name] = tex;

	return;
}

sf::Texture& TextureManager::getRef(const std::string &texture)
{
	return this->textures.at(texture);
}
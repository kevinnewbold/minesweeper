#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

std::unordered_map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::getTexture(std::string textureName)
{
	auto result = textures.find(textureName);
	if (result == textures.end())
	{
		sf::Texture newTexture;
		newTexture.loadFromFile("images/" + textureName + ".png");
		textures[textureName] = newTexture;
		return textures[textureName];
	}
	else
	{
		return result->second;
	}
}

sf::Texture& TextureManager::getNumberTexture(int number)
{
	if (number == 1)
		return getTexture("number_1");
	else if (number == 2)
		return getTexture("number_2");
	else if (number == 3)
		return getTexture("number_3");
	else if (number == 4)
		return getTexture("number_4");
	else if (number == 5)
		return getTexture("number_5");
	else if (number == 6)
		return getTexture("number_6");
	else if (number == 7)
		return getTexture("number_7");
	else if (number == 8)
		return getTexture("number_8");
	else if (number == 9)
		return getTexture("mine");
	else if (number == 0)
		return getTexture("tile_revealed");
}

TextureManager::TextureManager()
{
	
}
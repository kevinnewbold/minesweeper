#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <vector>

enum class State
{
	CLEAR = 0,
	ONE = 1,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	MINE
};

class Tile
{
	sf::Texture m_texture;
	sf::Texture m_secondaryTexture;
	sf::Sprite m_overlaySprite;
	sf::Sprite m_flagSprite;
	sf::Sprite m_sprite;
	std::vector<Tile*> m_neighbors;
	State m_state = State::CLEAR;
public:
	bool secondarySpriteOn = false;
	bool flagged = false;
	bool tileRevealed = false;
	Tile();
	Tile(State state);
	Tile(State state, sf::Texture& tex, float posX, float posY);
	~Tile();
	void setNeighbor(Tile* neighbor);
	std::vector<Tile*> getNeighbors();
	void toggleFlag(int& correctFlags, int& flagCount);
	void revealTile();
	State getState();
	void setState(State s);
	sf::Sprite& getSprite();
	sf::Texture& getSecondaryTexture();
	void setSecondaryTexture();
	void setOverlaySprite();
	sf::Sprite& getOverlaySprite();
	void drawTile(sf::RenderWindow& window, bool timerOn, bool gameWon, bool leaderboardOn);
	void revealNeighbors();
	sf::Texture& getTexture();
	void setTexture(sf::Texture& tex);
};
#include "Tile.h"
#include <iostream>
Tile::Tile()
{
	m_state = State::CLEAR;
}

Tile::Tile(State state)
{
	m_state = state;
}

Tile::Tile(State state, sf::Texture& tex, float posX, float posY)
{
	m_state = state;
	m_texture = tex;
	m_sprite.setTexture(tex);
	m_sprite.setPosition(posX, posY);
	m_overlaySprite.setPosition(posX, posY);
	m_flagSprite.setPosition(posX, posY);
	m_flagSprite.setTexture(TextureManager::getTexture("flag"));
}

Tile::~Tile()
{
	/*for (int i = 0; i < m_neighbors.size(); i++)
	{
		Tile* t = m_neighbors.at(i);
		delete t;
	}
	m_neighbors.clear();*/
}

void Tile::toggleFlag(int& correctFlags, int& flagCount)
{
	flagged = !flagged;
	if (flagged && m_state == State::MINE)
	{
		correctFlags++;
		flagCount--;
	}
	else if (!flagged && m_state == State::MINE)
	{
		correctFlags--;
		flagCount++;
	}
	else if (flagged)
		flagCount--;
	else if (!flagged)
		flagCount++;

}

void Tile::revealTile()
{

}
// Set starting from right neighbor, clockwise around
void Tile::setNeighbor(Tile* neighbor)
{
	m_neighbors.push_back(neighbor);
}

std::vector<Tile*> Tile::getNeighbors()
{
	return m_neighbors;
}

State Tile::getState()
{
	return m_state;
}

void Tile::setState(State s)
{
	m_state = s;
}

sf::Sprite& Tile::getSprite()
{
	return m_sprite;
}

sf::Texture& Tile::getSecondaryTexture()
{
	return m_secondaryTexture;
}

sf::Texture& Tile::getTexture()
{
	return m_texture;
}

void Tile::setTexture(sf::Texture& tex)
{
	m_texture = tex;
}

void Tile::setSecondaryTexture()
{
	m_secondaryTexture = TextureManager::getNumberTexture(static_cast<int>(m_state));
}

void Tile::setOverlaySprite() 
{
	m_overlaySprite.setTexture(m_secondaryTexture);
}

sf::Sprite& Tile::getOverlaySprite()
{
	return m_overlaySprite;
}

void Tile::drawTile(sf::RenderWindow& window, bool timerOn, bool gameWon, bool leaderboardOn)
{
	window.draw(m_sprite);

	if (secondarySpriteOn)
	{
		window.draw(m_overlaySprite);
	}
	if (flagged && timerOn && !leaderboardOn)
	{
		window.draw(m_flagSprite);
	}
	if (gameWon && m_state == State::MINE && !leaderboardOn)
		window.draw(m_flagSprite);

}

void Tile::revealNeighbors()
{
	if (m_state == State::CLEAR)
	{
		for (int i = 0; i < m_neighbors.size(); i++)
		{
			if (m_neighbors.at(i) != nullptr && !m_neighbors.at(i)->tileRevealed && !m_neighbors.at(i)->flagged)
			{
				m_neighbors.at(i)->secondarySpriteOn = true;
				m_neighbors.at(i)->tileRevealed = true;
				m_neighbors.at(i)->getSprite().setTexture(TextureManager::getTexture("tile_revealed"));
				m_neighbors.at(i)->setTexture(TextureManager::getTexture("tile_revealed"));
				m_neighbors.at(i)->revealNeighbors();
			}

		}
	}
}
#pragma once
#include <vector>
#include "Tile.h"
#include "Leaderboard.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <math.h>
#include <chrono>

class Board
{
	int m_rows = 0;
	int m_cols = 0;
	int m_size = 0;
	int m_mines = 0;
	std::vector<Tile> m_tiles;
	sf::Sprite faceButton;
	sf::Sprite counter[3] = {};
	sf::Sprite debug;
	sf::Sprite leaderboard;
	sf::Sprite pausePlay;
	sf::Sprite timerMinutes[2] = {};
	sf::Sprite timerSeconds[2] = {};

public:
	bool debugModeActive = false;
	bool gameOn = true;
	bool gameWon = false;
	bool leaderboardOpen = false;
	int correctFlagCount = 0;
	int flagsLeft;
	std::string name;
	std::vector<int> winningTimes;
	std::chrono::duration<double> timeTotal;
	std::chrono::seconds timeSeconds;
	std::chrono::minutes timeMinutes;
	bool timerOn = true;
	Board();
	Board(int rows, int cols, int mines, sf::Texture& tex);
	void setMines();
	void setNumbers();
	void setNeighbors();
	void drawTiles(sf::RenderWindow& window);
	void drawUI(sf::RenderWindow& window);
	void resetBoard();
	void setTimer(int minutes, int seconds);
	void setCounter(int flags);
	void toggleDebugMode();
	void togglePause();
	void gameOver();
	void gameWin(sf::RenderWindow& window);
	void pauseTimer();
	void openLeaderboard(sf::RenderWindow& window);
	void interpretLeftClickToButton(sf::Vector2f position, sf::RenderWindow& window);
	void interpretRightClickToButton(sf::Vector2f position, sf::RenderWindow& window);
	void checkTileClicked(sf::Vector2f position, sf::RenderWindow& window, std::string LorR);
	void setTilesRevealed(bool b);
	void recordTimes();
};
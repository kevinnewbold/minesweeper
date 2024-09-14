#include "Board.h"
Board::Board()
{
	
}

Board::Board(int rows, int cols, int mines, sf::Texture& tex)
{
	m_rows = rows;
	m_cols = cols;
	m_mines = mines;
	m_size = rows * cols;
	flagsLeft = m_mines;
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_cols; j++)
		{
			Tile t(State::CLEAR, tex, (j) * 32, (i) * 32);
			m_tiles.push_back(t);
		}
	}
	faceButton.setTexture(TextureManager::getTexture("face_happy"));
	faceButton.setPosition((m_cols / 2.0f * 32) - 32, 32 * (m_rows + 0.5f));
	debug.setTexture(TextureManager::getTexture("debug"));
	debug.setPosition((m_cols * 32) - 304, 32 * (m_rows + 0.5f));
	leaderboard.setTexture(TextureManager::getTexture("leaderboard"));
	leaderboard.setPosition((m_cols * 32) - 176, 32 * (m_rows + 0.5f));
	pausePlay.setTexture(TextureManager::getTexture("pause"));
	pausePlay.setPosition((m_cols * 32) - 240, 32 * (m_rows + 0.5f));
	setTimer(0, 0);
	timerMinutes[0].setPosition((m_cols * 32) - 97, 32 * (m_rows + 0.5f) + 16);
	timerMinutes[1].setPosition((m_cols * 32) - 97 + 21, 32 * (m_rows + 0.5f) + 16);
	timerSeconds[0].setPosition((m_cols * 32) - 54, 32 * (m_rows + 0.5f) + 16);
	timerSeconds[1].setPosition((m_cols * 32) - 54 + 21, 32 * (m_rows + 0.5f) + 16);
	setCounter(m_mines);
	counter[0].setPosition(33, 32 * (m_rows + 0.5f) + 16);
	counter[1].setPosition(33 + 21, 32 * (m_rows + 0.5f) + 16);
	counter[2].setPosition(33 + 42, 32 * (m_rows + 0.5f) + 16);
	timeSeconds = std::chrono::seconds(0);
}

void Board::setTimer(int minutes, int seconds)
{
	timerMinutes[0].setTexture(TextureManager::getTexture("digits"));
	timerMinutes[1].setTexture(TextureManager::getTexture("digits"));
	timerSeconds[0].setTexture(TextureManager::getTexture("digits"));
	timerSeconds[1].setTexture(TextureManager::getTexture("digits"));
	timerMinutes[0].setTextureRect(sf::IntRect((minutes / 10 * 21), 0, 21, 32));
	timerMinutes[1].setTextureRect(sf::IntRect((minutes % 10 * 21), 0, 21, 32));
	timerSeconds[0].setTextureRect(sf::IntRect((seconds / 10 * 21), 0, 21, 32));
	timerSeconds[1].setTextureRect(sf::IntRect((seconds % 10 * 21), 0, 21, 32));
}

void Board::setCounter(int flags)
{
	counter[0].setTexture(TextureManager::getTexture("digits"));
	counter[1].setTexture(TextureManager::getTexture("digits"));
	counter[2].setTexture(TextureManager::getTexture("digits"));
	if (flags >= 0)
		counter[0].setTextureRect(sf::IntRect((flags / 100 * 21), 0, 21, 32));
	else
		counter[0].setTextureRect(sf::IntRect((10 * 21), 0, 21, 32));
	counter[1].setTextureRect(sf::IntRect(abs(flags / 10 * 21), 0, 21, 32));
	counter[2].setTextureRect(sf::IntRect(abs(flags % 10 * 21), 0, 21, 32));
	
}

void Board::setMines()
{
	srand((unsigned) time(NULL));
	int minesSet = 0;
	while (minesSet < m_mines)
	{
		int place = rand() % m_size;
		if (m_tiles[place].getState() != State::MINE)
		{
			m_tiles[place].setState(State::MINE);
			minesSet++;
		}
	}
}
// set starting from mid right, counterclockwise around 
void Board::setNeighbors()
{
	for (int i = 0; i < m_size; i++)
	{
		// Mid Right
		if (((i + 1) % m_cols != 0))
			m_tiles.at(i).setNeighbor(&m_tiles.at(i + 1));
		// Bottom right
		if (((i + 1) % m_cols != 0) && i < ((m_rows - 1) * (m_cols)))
			m_tiles.at(i).setNeighbor(&m_tiles.at(i + m_cols + 1));
		// Bottom mid
		if(i < ((m_rows - 1) * (m_cols)))
			m_tiles.at(i).setNeighbor(&m_tiles.at(i + m_cols));
		// Bottom left
		if((i) % m_cols != 0 && i < ((m_rows - 1) * (m_cols)))
			m_tiles.at(i).setNeighbor(&m_tiles.at(i + m_cols - 1));
		// Mid left
		if((i) % m_cols != 0)
			m_tiles.at(i).setNeighbor(&m_tiles.at(i - 1));
		// Top left
		if ((i) % m_cols != 0 && i - m_cols > 0)
		{
			m_tiles.at(i).setNeighbor(&m_tiles.at(i - m_cols - 1));
		}
		// Top mid
		if((i - m_cols) > 0)
			m_tiles.at(i).setNeighbor(&m_tiles.at(i - m_cols));
		// Top right
		if ((i - m_cols) > 0 && ((i + 1) % m_cols != 0))
			m_tiles.at(i).setNeighbor(&m_tiles.at(i - m_cols + 1));
	}
}

void Board::setNumbers()
{
	for (int i = 0; i < m_size; i++)
	{
		int mineCount = 0;
		for (int j = 0; j < m_tiles.at(i).getNeighbors().size(); j++)
		{
			if (m_tiles.at(i).getNeighbors().at(j)->getState() == State::MINE)
				mineCount++;
		}
		if (m_tiles.at(i).getState() != State::MINE)
		{
			m_tiles.at(i).setState(static_cast<State>(mineCount));
			m_tiles.at(i).setSecondaryTexture();
			m_tiles.at(i).setOverlaySprite();
		}
		else
		{
			m_tiles.at(i).setSecondaryTexture();
			m_tiles.at(i).setOverlaySprite();
		}

	}
}

void Board::resetBoard()
{
	timeTotal = std::chrono::duration<double>::zero();
	timeMinutes = std::chrono::minutes::zero();
	timeSeconds = std::chrono::seconds::zero();
	correctFlagCount = 0;
	flagsLeft = m_mines;
	gameOn = true;
	gameWon = false;
	debugModeActive = false;
	timerOn = true;
	m_tiles.clear();
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_cols; j++)
		{
			Tile t(State::CLEAR, TextureManager::getTexture("tile_hidden"), (j) * 32, (i) * 32);
			m_tiles.push_back(t);
		}
	}
	setMines();
	setNeighbors();
	setNumbers();
	setTimer(0, 0);
	setCounter(m_mines);
	faceButton.setTexture(TextureManager::getTexture("face_happy"));
	pausePlay.setTexture(TextureManager::getTexture("pause"));
}

void Board::drawTiles(sf::RenderWindow& window)
{
	for (int i = 0; i < m_size; i++)
	{
		m_tiles.at(i).drawTile(window, timerOn, gameWon, leaderboardOpen);
	}
}

void Board::drawUI(sf::RenderWindow& window)
{
	window.draw(faceButton);
	window.draw(debug);
	window.draw(leaderboard);
	window.draw(pausePlay);
	window.draw(timerMinutes[0]);
	window.draw(timerMinutes[1]);
	window.draw(timerSeconds[0]);
	window.draw(timerSeconds[1]);
	window.draw(counter[0]);
	window.draw(counter[1]);
	window.draw(counter[2]);
}

void Board::toggleDebugMode()
{
	debugModeActive = !debugModeActive;
	if (debugModeActive)
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_tiles.at(i).getState() == State::MINE)
				m_tiles.at(i).secondarySpriteOn = true;
		}
	}
	else
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_tiles.at(i).getState() == State::MINE)
				m_tiles.at(i).secondarySpriteOn = false;
		}
	}
}

void Board::setTilesRevealed(bool b)
{
	if (b)
	{
		for (int i = 0; i < m_size; i++)
		{
			sf::Texture s = m_tiles.at(i).getTexture();
			m_tiles.at(i).setTexture(s);
			m_tiles.at(i).getSprite().setTexture(TextureManager::getTexture("tile_revealed"));
			m_tiles.at(i).secondarySpriteOn = false;
		}
	}
	else
	{
		for (int i = 0; i < m_size; i++)
		{
			m_tiles.at(i).getSprite().setTexture(m_tiles.at(i).getTexture());
			if(m_tiles.at(i).tileRevealed)
				m_tiles.at(i).secondarySpriteOn = true;
			if (debugModeActive == true)
			{
				debugModeActive = !debugModeActive;
				toggleDebugMode();
			}
		}
	}
}

void Board::openLeaderboard(sf::RenderWindow& window)
{
	leaderboardOpen = true;
	const auto startWindow = std::chrono::high_resolution_clock::now();
	setTilesRevealed(true);
	drawTiles(window);
	window.display();
	int bestTime = -1;
	for (int i = 0; i < winningTimes.size(); i++)
	{
		if (winningTimes.at(i) > bestTime)
			bestTime = winningTimes.at(i);
	}
	Leaderboard newLeaderboard(m_rows, m_cols, name, bestTime);
	if(timerOn || !gameOn)
		setTilesRevealed(false);
	drawTiles(window);
	window.display();
	const auto endWindow = std::chrono::high_resolution_clock::now();
	if(timerOn)
		timeTotal -= std::chrono::duration<double>(endWindow - startWindow);
	leaderboardOpen = false;
}

void Board::togglePause()
{
	timerOn = !timerOn;
	if (!timerOn)
	{
		pausePlay.setTexture(TextureManager::getTexture("play"));
		pauseTimer();
		setTilesRevealed(true);
	}
	else
	{
		pausePlay.setTexture(TextureManager::getTexture("pause"));
		setTilesRevealed(false);
	}
}

void Board::pauseTimer()
{

}

void Board::gameOver()
{
	gameOn = false;
	timerOn = false;
	faceButton.setTexture(TextureManager::getTexture("face_lose"));
	for (int i = 0; i < m_size; i++)
	{
		if (m_tiles.at(i).getState() == State::MINE)
			m_tiles.at(i).secondarySpriteOn = true;
	}
}
void Board::gameWin(sf::RenderWindow& window)
{
	for (int j = 0; j < m_size; j++)
	{
		if (m_tiles.at(j).getState() == State::MINE)
			m_tiles.at(j).flagged = true;
	}
	gameWon = true;
	gameOn = false;
	timerOn = false;
	faceButton.setTexture(TextureManager::getTexture("face_win"));
	winningTimes.push_back(std::chrono::duration_cast<std::chrono::seconds>(timeTotal).count());
}

void Board::interpretLeftClickToButton(sf::Vector2f position, sf::RenderWindow& window)
{
	if (faceButton.getGlobalBounds().contains(position))
		resetBoard();
	else if (debug.getGlobalBounds().contains(position) && gameOn && timerOn)
		toggleDebugMode();
	else if (leaderboard.getGlobalBounds().contains(position))
		openLeaderboard(window);
	else if (pausePlay.getGlobalBounds().contains(position) && gameOn)
		togglePause();
	else
		checkTileClicked(position, window, "L");
}

void Board::interpretRightClickToButton(sf::Vector2f position, sf::RenderWindow& window)
{
	checkTileClicked(position, window, "R");
}

void Board::checkTileClicked(sf::Vector2f position, sf::RenderWindow& window, std::string LorR)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_tiles.at(i).getSprite().getGlobalBounds().contains(position))
		{
			if (LorR == "L" && !m_tiles.at(i).flagged && gameOn && timerOn)
			{
				std::cout << i << std::endl;
				m_tiles.at(i).secondarySpriteOn = true;
				m_tiles.at(i).tileRevealed = true;
				m_tiles.at(i).getSprite().setTexture(TextureManager::getTexture("tile_revealed"));
				m_tiles.at(i).setTexture(TextureManager::getTexture("tile_revealed"));
				m_tiles.at(i).revealNeighbors();
				if (m_tiles.at(i).getState() == State::MINE)
					gameOver();
				int tilesRevealed = 0;
				for (int j = 0; j < m_size; j++)
				{
					if(m_tiles.at(j).tileRevealed)
						tilesRevealed++;
				}
				if (tilesRevealed == m_size - m_mines && gameOn)
				{
					gameWin(window);

				}
			}
			if (LorR == "R" && !m_tiles.at(i).tileRevealed && gameOn && timerOn)
			{
				m_tiles.at(i).toggleFlag(correctFlagCount, flagsLeft);
				std::cout << flagsLeft;
				setCounter(flagsLeft);
			}
		}
			
	}
}

void Board::recordTimes()
{
	if (winningTimes.size() > 0)
	{
		int winningTime = -1;
		for (int i = 0; i < winningTimes.size(); i++)
		{
			if (winningTimes.at(i) > winningTime)
				winningTime = winningTimes.at(i);
		}
		Leaderboard l(name, winningTime);

	}
}
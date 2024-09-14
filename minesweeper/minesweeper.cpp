#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"
#include "TextureManager.h"
#include "Leaderboard.h"

void setText(sf::Text& text, float x, float y, sf::Color c)
{
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(x, y));
	text.setFillColor(c);
}

void setText(sf::Text& text, float x, float y)
{
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(x, y));
}


void getConfig(int& rows, int& cols, int& mines)
{
	std::ifstream config("config.cfg");
	if (!config.is_open())
		std::cout << "config aint open, chief";
	std::string line;
	getline(config, line);
	rows = stoi(line);
	getline(config, line);
	cols = stoi(line);
	getline(config, line);
	mines = stoi(line);
}

int main()
{
	// General Setup
	int rows = 0, cols = 0, mines = 0;
	getConfig(cols, rows, mines);
	sf::Font font;
	if (!font.loadFromFile("font.ttf"))
		std::cout << "error: font ain't here";
	font.loadFromFile("font.ttf");
	bool beginGame = false;

	// Welcome Window
	sf::RenderWindow welcomeWindow(sf::VideoMode(cols * 32, rows * 32 + 100), "Minesweeper", sf::Style::Close);
	sf::Text welcome("WELCOME TO MINESWEEPER", font, 24);
	sf::Text enterName("Enter your name:", font, 20);
	sf::Text input("|", font, 18);
	setText(welcome, welcomeWindow.getSize().x / 2.0f, welcomeWindow.getSize().y / 2.0f - 150.0f, sf::Color::White);
	setText(enterName, welcomeWindow.getSize().x / 2.0f, welcomeWindow.getSize().y / 2.0f - 75.0f, sf::Color::White);
	setText(input, welcomeWindow.getSize().x / 2.0f, welcomeWindow.getSize().y / 2.0f - 45.0f, sf::Color::Yellow);
	welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
	enterName.setStyle(sf::Text::Bold);
	input.setStyle(sf::Text::Bold);
	std::string name;
	while (welcomeWindow.isOpen())
	{
		sf::Event event;
		while (welcomeWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				welcomeWindow.close();
			if (event.type == sf::Event::TextEntered)
			{
				if (std::isalpha(event.text.unicode) != 0 && name.length() < 10)
					name += event.text.unicode;
				if (name.length() > 0)
				{
					name.at(0) = std::toupper(name.at(0));
					for (int i = 1; i < name.length(); i++)
					{
						name.at(i) = std::tolower(name.at(i));
					}
					input.setString(name + "|");
					setText(input, welcomeWindow.getSize().x / 2.0f, welcomeWindow.getSize().y / 2.0f - 45.0f);
				}

			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.scancode == sf::Keyboard::Scan::Backspace)
				{
					if (name.length() > 0)
						name.pop_back();
					input.setString(name + "|");
					setText(input, welcomeWindow.getSize().x / 2.0f, welcomeWindow.getSize().y / 2.0f - 45.0f);
				}
				if (event.key.scancode == sf::Keyboard::Scan::Enter)
				{
					if (name.length() > 0)
					{
						welcomeWindow.close();
						beginGame = true;
					}
				}
			}
		}
		welcomeWindow.clear(sf::Color::Blue);

		welcomeWindow.draw(welcome);
		welcomeWindow.draw(enterName);
		welcomeWindow.draw(input);
		welcomeWindow.display();
	}

	// Game Window
	if (beginGame)
	{
		sf::RenderWindow gameWindow(sf::VideoMode(cols * 32, rows * 32 + 100), "Minesweeper", sf::Style::Close);
		Board b(rows, cols, mines, TextureManager::getTexture("tile_hidden"));
		b.setMines();
		b.setNeighbors();
		b.setNumbers();
		b.name = name;
		while (gameWindow.isOpen())
		{
			const auto startLoop = std::chrono::high_resolution_clock::now();
			sf::Event event;
			while (gameWindow.pollEvent(event))
			{

				if (event.type == sf::Event::Closed)
				{
					b.recordTimes();
					gameWindow.close();
				}
				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
						sf::Vector2f pixelCoords = gameWindow.mapPixelToCoords(mousePosition);
						b.interpretLeftClickToButton(pixelCoords, gameWindow);
					}
					else if (event.mouseButton.button == sf::Mouse::Right)
					{
						sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
						sf::Vector2f pixelCoords = gameWindow.mapPixelToCoords(mousePosition);
						b.interpretRightClickToButton(pixelCoords, gameWindow);
					}
				}

			}

			gameWindow.clear(sf::Color::White);
			b.drawTiles(gameWindow);
			b.drawUI(gameWindow);
			gameWindow.display();
			const auto endLoop = std::chrono::high_resolution_clock::now();
			if (b.timerOn)
			{
				b.timeTotal += std::chrono::duration<double>(endLoop - startLoop);
				b.timeMinutes = std::chrono::duration_cast<std::chrono::minutes>(b.timeTotal);
				b.timeSeconds = std::chrono::duration_cast<std::chrono::seconds>(b.timeTotal) - std::chrono::duration_cast<std::chrono::seconds>(b.timeMinutes);
				b.setTimer(b.timeMinutes.count(), b.timeSeconds.count());
			}
		}


	}

	return 0;
}
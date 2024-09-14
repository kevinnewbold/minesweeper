#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
class Leaderboard
{

public:
	Leaderboard(int rows, int cols, std::string name, int time);
	Leaderboard(std::string name, int time);
	void setText(sf::Text& text, float x, float y);
	void readFile();
	void writeFile();
	sf::RenderWindow window;
	sf::Color color = sf::Color::Blue;
	sf::Text leaderboard;
	sf::Text leaderboardContent;
	sf::Font font;
	std::string leaderboardData;
	std::map<int, std::string> times;
	std::string newName;
	int winningTime;
};
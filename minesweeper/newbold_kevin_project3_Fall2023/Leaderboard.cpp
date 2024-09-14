#include "Leaderboard.h"

Leaderboard::Leaderboard(int rows, int cols, std::string name, int time)
{
	newName = name;
	winningTime = time;

	readFile();

	window.create(sf::VideoMode((cols * 16), rows * 16 + 50), "Leaderboard", sf::Style::Close);

	font.loadFromFile("font.ttf");
	leaderboard.setString("LEADERBOARD");
	leaderboard.setFont(font);
	leaderboard.setFillColor(sf::Color::White);
	leaderboard.setCharacterSize(20);
	setText(leaderboard, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 120.0f);
	leaderboard.setStyle(sf::Text::Bold | sf::Text::Underlined);
	leaderboardContent.setString(leaderboardData);
	leaderboardContent.setFont(font);
	leaderboardContent.setFillColor(sf::Color::White);
	leaderboardContent.setCharacterSize(18);
	setText(leaderboardContent, window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20.0f);
	leaderboardContent.setStyle(sf::Text::Bold);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Blue);

		window.draw(leaderboard);
		window.draw(leaderboardContent);
		window.display();
	}
	writeFile();
}

Leaderboard::Leaderboard(std::string name, int time)
{
	newName = name;
	winningTime = time;
	readFile();
	writeFile();
}

void Leaderboard::setText(sf::Text& text, float x, float y)
{
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(x, y));
}

void Leaderboard::readFile()
{
	std::ifstream file("leaderboard.txt");
	std::string s;

	while (getline(file, s))
	{
		std::string numbers = s.substr(0, 5);
		std::string name = s.substr(6);
		int seconds = stoi(numbers.substr(0, 2)) * 60 + stoi(numbers.substr(3));
		times[seconds] = name;
	}

	if (winningTime >= 0)
	{
		auto iter = times.begin();

		for (; iter != times.end(); ++iter)
		{
			if (winningTime < iter->first)
				times[winningTime] = newName + "*";
		}
	}
	auto iter = times.begin();
	for (int c = 1; c <= 5; c++)
	{
		std::string minutes = std::to_string(iter->first / 60);
		if (minutes.size() < 2)
			minutes = "0" + minutes;
		std::string seconds = std::to_string(iter->first % 60);
		if (seconds.size() < 2)
			seconds = "0" + seconds;
		std::string a = std::to_string(c) + "." + "\t";
		leaderboardData += (a + minutes + ":" + seconds + "\t" + iter->second + "\n\n");
		iter++;
	}


}

void Leaderboard::writeFile()
{
	auto iter = times.begin();
	std::ofstream file("leaderboard.txt");
	for (int i = 0; i < 5; i++)
	{
		std::string minutes = std::to_string(iter->first / 60);
		if (minutes.size() < 2)
			minutes = "0" + minutes;
		std::string seconds = std::to_string(iter->first % 60);
		if (seconds.size() < 2)
			seconds = "0" + seconds;
		std::string newstr = minutes + ":" + seconds + "," + iter->second;
		file << newstr << std::endl;
		iter++;
	}
}

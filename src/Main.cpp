#include "Main.hpp"
#include "KeyRead.hpp"
#include "Kana.hpp"
#include <iostream>
#include <random>
#include <fstream>

const float timeout = 3.f;

void fail(sf::Text& failText, int index) {
	failText.setString(romaji[index % nTotal]);
	sf::Rect textBounds = failText.getLocalBounds();
	failText.setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height) / 2.f);
}

int main()
{
	float weights[2*nTotal];

	std::fstream fs;
	fs.open("data/weights.bin", std::ios::binary | std::ios::in | std::ios::ate);
	
	if (fs.is_open() && fs.tellg() == 2*nTotal*sizeof(float)) {
		fs.seekg(0);
		fs.read(reinterpret_cast<char*>(weights), 2*nTotal*sizeof(float));
		fs.close();
	}
	else {
		for (int i = 0; i < 2*nTotal; i++)
		weights[i] = 1.f;
	}
	
	fs.close();

	std::discrete_distribution<int> dd(weights, weights + nTotal);
	std::default_random_engine gen(time(NULL));

	sf::RenderWindow window(sf::VideoMode(900, 800), "SFML works!");
	sf::Vector2f winSize = (sf::Vector2f) window.getSize();
	window.setFramerateLimit(60);

	sf::Event haps;

	sf::Font computer;
	computer.loadFromFile("resources/fonts/HanSerif.otf");
	sf::Font calligraphy;
	calligraphy.loadFromFile("resources/fonts/nagayama.otf");
	sf::Font hand1;
	hand1.loadFromFile("resources/fonts/mitsu.ttf");
	sf::Font hand2;
	hand2.loadFromFile("resources/fonts/sanafon.ttf");

	const int nFonts = 4;
	sf::Font* fonts[nFonts] {&computer, &calligraphy, &hand1, &hand2};

	sf::Text question;
	question.setCharacterSize(192);
	question.setPosition(winSize.x / 2, winSize.y / 4);

	sf::Font openSans;
	openSans.loadFromFile("resources/fonts/OpenSans.ttf");

	sf::Text answer;
	answer.setFont(openSans);
	answer.setCharacterSize(96);
	answer.setPosition(winSize.x / 2, 2 * winSize.y / 3);

	bool failed = false;
	sf::Text failAnswer;
	failAnswer.setFont(openSans);
	failAnswer.setCharacterSize(48);
	failAnswer.setPosition(winSize.x / 2, 4 * winSize.y / 5);
	failAnswer.setString("");
	failAnswer.setFillColor(sf::Color(50, 180, 80));

	int index;

	std::function<void()> changeChar = [&]() {
		question.setFont(*fonts[rand() % nFonts]);
		index = dd(gen);
		wchar_t chosenChar = kana[index];
		question.setString(chosenChar);

		sf::Rect textBounds = question.getLocalBounds();
		question.setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height) / 2.f);
	};

	changeChar();

	sf::Clock clock;

	while (window.isOpen())
	{
		while (window.pollEvent(haps))
		{
			if (haps.type == sf::Event::Closed)
				window.close();
			else if (haps.type == sf::Event::KeyPressed) {
				sf::String ans = answer.getString();
				if (haps.key.code == sf::Keyboard::BackSpace && ans.getSize() > 0) {
					answer.setString(ans.substring(0, ans.getSize() - 1));
					if (!failed) {
						fail(failAnswer, index);
						failed = true;
					}
				}
				else {
					answer.setString(ans + keyToStr(haps.key.code));
				}
				sf::Rect textBounds = answer.getLocalBounds();
				answer.setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height) / 2.f);

				ans = answer.getString();
				if (ans == romaji[index]) {
					float newWeight;
					if (failed) {
						failAnswer.setString("");
						failed = false;
						newWeight = weights[index] * 2.f;
						newWeight = fminf(newWeight, 10);
					}
					else {
						newWeight = weights[index] / 1.5f;
						newWeight = fmaxf(newWeight, 1.f);
					}

					int oldIndex = index;
					weights[index] = 0.f; //Temporary weight of 0 to prevent instant repetition
					dd = std::discrete_distribution(weights, weights + nTotal);
					changeChar();
					weights[oldIndex] = newWeight;

					answer.setString("");
					clock.restart();
				}
			}
		}

		if (clock.getElapsedTime().asSeconds() > timeout && !failed) {
			fail(failAnswer, index);
			failed = true;
		}

		window.clear();
		window.draw(question);
		window.draw(answer);
		window.draw(failAnswer);
		window.display();
	}
	
	fs.open("data/weights.bin", std::ios::binary | std::ios::out | std::ios::trunc);
	if (fs.is_open()) {
		fs.write(reinterpret_cast<char*>(weights), 2*nTotal*sizeof(float));
	}
	fs.close();

	return 0;
}

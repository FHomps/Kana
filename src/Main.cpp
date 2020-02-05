#include "Main.hpp"
#include "KeyRead.hpp"
#include "Kana.hpp"
#include "Card.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <queue>

const float timeout = 2.f;

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

	std::discrete_distribution<int> dd(weights, weights + 2*nTotal);
	std::default_random_engine gen(time(NULL));

	sf::RenderWindow window(sf::VideoMode(900, 800), "Kana trainer");
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

	sf::Font openSans;
	openSans.loadFromFile("resources/fonts/OpenSans.ttf");

	const int trail = 2;
	Card* questions[2 * trail + 1];
	for (int i = 0; i < 2*trail+1; i++)
		questions[i] = nullptr;
	questions[trail] = new Card(dd(gen), fonts[rand() % nFonts], true);
	questions[trail]->setPosition(winSize.x / 2, winSize.y / 4);
	for (int i = trail - 1; i >= 0; i--) {
		questions[i] = new Card(dd(gen), fonts[rand() % nFonts]);
		questions[i]->setPosition(winSize.x / 2 + 100 + 100*(trail-i), winSize.y / 4);
	}

	int currentIndex = questions[trail]->getIndex();

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

	sf::Clock clock;

	while (window.isOpen())
	{
		while (window.pollEvent(haps))
		{
			if (haps.type == sf::Event::Closed)
				window.close();
			else if (haps.type == sf::Event::KeyPressed) {
				bool stillMoving = false;
				for (int i = 0; i < 2*trail + 1; i++) {
					if (questions[i] != nullptr && questions[i]->isMoving()) {
						stillMoving = true;
						break;
					}
				}
				if (!stillMoving) {
					sf::String ans = answer.getString();
					if (haps.key.code == sf::Keyboard::BackSpace && ans.getSize() > 0) {
						answer.setString(ans.substring(0, ans.getSize() - 1));
						if (!failed) {
							fail(failAnswer, currentIndex);
							failed = true;
						}
					}
					else {
						answer.setString(ans + keyToStr(haps.key.code));
					}
					sf::Rect textBounds = answer.getLocalBounds();
					answer.setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height) / 2.f);

					ans = answer.getString();
					if (ans == romaji[currentIndex % nTotal]) {
						if (failed) {
							failAnswer.setString("");
							failed = false;

							weights[currentIndex] = fminf(weights[currentIndex] * 2.f, 10.f);
						}
						else {
							weights[currentIndex] = fmaxf(weights[currentIndex] / 1.5f, .25f);
						}

						//Advance the card queue
						if (questions[2*trail] != nullptr) {
							delete questions[2*trail];
						}
						for (int i = 2*trail; i > 0; i--) {
							questions[i] = questions[i-1];
							if (questions[i] != nullptr)
								questions[i]->startMoving(i == trail);
						}
						currentIndex = questions[trail]->getIndex();

						//Generate a new card
						dd = std::discrete_distribution(weights, weights + 2*nTotal);
						questions[0] = new Card(dd(gen), fonts[rand() % nFonts]);
						questions[0]->setPosition(winSize.x / 2 + 200 + 100*trail, winSize.y / 4);
						questions[0]->startMoving();

						answer.setString("");
						clock.restart();
					}
				}
			}
		}

		if (clock.getElapsedTime().asSeconds() > timeout && !failed) {
			fail(failAnswer, currentIndex);
			failed = true;
		}

		window.clear();
		for (int i = 0; i < 2*trail+1; i++) {
			if (questions[i] != nullptr) {
				questions[i]->update();
				window.draw(*questions[i]);
			}
		}
		
		window.draw(answer);
		window.draw(failAnswer);
		window.display();
	}

	for (int i = 0; i < 2*trail + 1; i++) {
		if (questions[i] != nullptr)
			delete questions[i];
	}
	
	fs.open("data/weights.bin", std::ios::binary | std::ios::out | std::ios::trunc);
	if (fs.is_open()) {
		fs.write(reinterpret_cast<char*>(weights), 2*nTotal*sizeof(float));
	}
	fs.close();

	return 0;
}

#include "Main.hpp"
#include "KeyRead.hpp"
#include "Kana.hpp"
#include "Card.hpp"
#include "Results.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <queue>
#include <filesystem>
namespace fs = std::filesystem;

const float timeout = 2.f;

void fail(sf::Text& failText, int index) {
	failText.setString(romaji[index % nTotal]);
	sf::FloatRect textBounds = failText.getLocalBounds();
	failText.setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height) / 2.f);
}

sf::Font const& randomFont(std::map<std::string, sf::Font> const& fonts) {
	std::map<std::string, sf::Font>::const_iterator it = fonts.begin();
	std::advance(it, rand() % fonts.size());
	return it->second;
}

int main()
{	
	std::fstream file;
	file.open("resources/fonts/kana/defaultFont.txt");
	std::string defaultFontName;
	if (file.is_open()) {
		std::getline(file, defaultFontName);
		file.close();
	}

	std::map<std::string, sf::Font> fonts;
	sf::Font* defaultKanaFont = nullptr;
	for (const fs::directory_entry& entry : fs::directory_iterator("resources/fonts/kana/")) {
		if (entry.path().extension() == ".otf" || entry.path().extension() == ".ttf") {
			if (!fonts[entry.path().filename()].loadFromFile(entry.path().string())) {
				fonts.erase(entry.path().filename());
			}
			else if (entry.path().string() == defaultFontName) {
				defaultKanaFont = &fonts[entry.path().filename()];
			}
		}
	}
	if (defaultKanaFont == nullptr) {
		defaultKanaFont = &fonts.begin()->second;
	}
	
	for (std::pair<const std::string, sf::Font>& entry : fonts) {
		std::cout << entry.first << std::endl;
	}
	
	sf::Font romajiFont;
	for (const fs::directory_entry& entry : fs::directory_iterator("resources/fonts/romaji/")) {
		if (entry.path().extension() == ".otf" || entry.path().extension() == ".ttf") {
			if (romajiFont.loadFromFile(entry.path().string())) {
				break;
			}
		}
	}

	float weights[2*nTotal];

	file.open("data/weights.bin", std::ios::binary | std::ios::in | std::ios::ate);
	
	if (file.is_open() && file.tellg() == 2*nTotal*sizeof(float)) {
		file.seekg(0);
		file.read(reinterpret_cast<char*>(weights), 2*nTotal*sizeof(float));
		file.close();
	}
	else {
		for (int i = 0; i < 2*nTotal; i++)
		weights[i] = 1.f;
	}
	
	file.close();

	std::discrete_distribution<int> dd(weights, weights + 2*nTotal);
	std::default_random_engine gen(time(NULL));

	sf::RenderWindow window(sf::VideoMode(900, 700), "Kana Trainer");
	sf::Vector2f winSize = (sf::Vector2f) window.getSize();
	window.setFramerateLimit(60);

	sf::Event haps;

	const int trail = 2;
	Card* questions[2 * trail + 1];
	for (int i = 0; i < 2*trail+1; i++)
		questions[i] = nullptr;
	questions[trail] = new Card(dd(gen), randomFont(fonts), true);
	questions[trail]->setPosition(winSize.x / 2, winSize.y / 4);
	for (int i = trail - 1; i >= 0; i--) {
		questions[i] = new Card(dd(gen), randomFont(fonts));
		questions[i]->setPosition(winSize.x / 2 + 100 + 100*(trail-i), winSize.y / 4);
	}

	int currentIndex = questions[trail]->getIndex();

	sf::Text answer;
	answer.setFont(romajiFont);
	answer.setCharacterSize(96);
	answer.setPosition(winSize.x / 2, 2 * winSize.y / 3);

	bool failed = false;
	sf::Text failAnswer;
	failAnswer.setFont(romajiFont);
	failAnswer.setCharacterSize(48);
	failAnswer.setPosition(winSize.x / 2, 4 * winSize.y / 5);
	failAnswer.setString("");
	failAnswer.setFillColor(sf::Color(50, 180, 80));

	sf::Clock clock;

	enum Screens {
		MAIN, RESULTS
	} screen = MAIN;

	Results results(*defaultKanaFont);
	sf::FloatRect resultsBounds = results.getLocalBounds();
	results.setOrigin(sf::Vector2f(resultsBounds.left + resultsBounds.width, resultsBounds.top + resultsBounds.height) / 2.f);
	results.setPosition(winSize.x / 2, winSize.y / 2);

	while (window.isOpen())
	{
		if (screen == MAIN) {
			while (window.pollEvent(haps)) {
				if (haps.type == sf::Event::Closed)
					window.close();
				else if (haps.type == sf::Event::KeyPressed) {
					if (haps.key.code == sf::Keyboard::Escape) {
						screen = RESULTS;
						results.update(&(weights[0]));
					}
					else {
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
							sf::FloatRect textBounds = answer.getLocalBounds();
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
								//Prevent consecutive appearance of the same kana
								float weightBuffer = weights[questions[1]->getIndex()];
								weights[questions[1]->getIndex()] = 0;
								dd = std::discrete_distribution<int>(weights, weights + 2*nTotal);
								questions[0] = new Card(dd(gen), randomFont(fonts));
								weights[questions[1]->getIndex()] = weightBuffer;

								questions[0]->setPosition(winSize.x / 2 + 200 + 100*trail, winSize.y / 4);
								questions[0]->startMoving();

								answer.setString("");
								clock.restart();
							}
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
		else if (screen == RESULTS) {
			while (window.pollEvent(haps)) {
				if (haps.type == sf::Event::Closed)
					window.close();
				else if (haps.type == sf::Event::KeyPressed && haps.key.code == sf::Keyboard::Escape) {
					screen = MAIN;
					clock.restart();
				}
			}

			window.clear(sf::Color::Black);
			window.draw(results);
			window.display();
		}
	}

	for (int i = 0; i < 2*trail + 1; i++) {
		if (questions[i] != nullptr)
			delete questions[i];
	}
	
	file.open("data/weights.bin", std::ios::binary | std::ios::out | std::ios::trunc);
	if (file.is_open()) {
		file.write(reinterpret_cast<char*>(weights), 2*nTotal*sizeof(float));
	}
	file.close();

	return 0;
}

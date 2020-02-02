#include "Main.hpp"
#include "KeyRead.hpp"
#include <iostream>
#include <random>
#include <fstream>

const int nKana = 45;
const int nDiac = 25;
const int nTotal = nKana + nDiac;

const char* romaji[] {
"a","ka","sa","ta","na","ha","ma","ya","ra","wa",
"i","ki","shi","chi","ni","hi","mi","ri",
"u","ku","su","tsu","nu","fu","mu","yu","ru",
"e","ke","se","te","ne","he","me","re",
"o","ko","so","to","no","ho","mo","yo","ro","wo",

"ga","chi","da","ba","pa",
"gi","ji","dji","bi","pi",
"gu","zu","dzu","bu","pu",
"ge","ze","de","be","pe",
"go","zo","do","bo","po"};

const wchar_t kana[] {
L'あ',L'か',L'さ',L'た',L'な',L'は',L'ま',L'や',L'ら',L'わ',
L'い',L'き',L'し',L'ち',L'に',L'ひ',L'み',		L'り',
L'う',L'く',L'す',L'つ',L'ぬ',L'ふ',L'む',L'ゆ',L'る',
L'え',L'け',L'せ',L'て',L'ね',L'へ',L'め',		L'れ',
L'お',L'こ',L'そ',L'と',L'の',L'ほ',L'も',L'よ',L'ろ',L'を',

L'が',L'ざ',L'だ',L'ば',L'ぱ',
L'ぎ',L'じ',L'ぢ',L'び',L'ぴ',
L'ぐ',L'ず',L'づ',L'ぶ',L'ぷ',
L'げ',L'ぜ',L'で',L'べ',L'ぺ',
L'ご',L'ぞ',L'ど',L'ぼ',L'ぽ',

L'ア',L'カ',L'サ',L'タ',L'ナ',L'ハ',L'マ',L'ヤ',L'ラ',L'ワ',
L'イ',L'キ',L'シ',L'チ',L'ニ',L'ヒ',L'ミ',		L'リ',
L'ウ',L'ク',L'ス',L'ツ',L'ヌ',L'フ',L'ム',L'ユ',L'ル',
L'エ',L'ケ',L'セ',L'テ',L'ネ',L'ヘ',L'メ',		L'レ',
L'オ',L'コ',L'ソ',L'ト',L'ノ',L'ホ',L'モ',L'ヨ',L'ロ',L'ヲ',

L'ガ',L'ザ',L'ダ',L'バ',L'パ',
L'ギ',L'ジ',L'ヂ',L'ビ',L'ピ',
L'グ',L'ズ',L'ヅ',L'ブ',L'プ',
L'ゲ',L'ゼ',L'デ',L'ベ',L'ペ',
L'ゴ',L'ゾ',L'ド',L'ボ',L'ポ'};

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

		for (int i = 0; i < 2*nTotal; i++) {
			std::cout << weights[i] << '\n';
		}
		std::cout << std::endl;
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

	std::vector<sf::Font*> fonts {&computer, &calligraphy, &hand1, &hand2};

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
		question.setFont(*fonts[rand() % fonts.size()]);
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

		if (clock.getElapsedTime().asSeconds() > 2 && !failed) {
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

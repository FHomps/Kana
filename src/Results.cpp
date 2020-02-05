#include "Results.hpp"

const float cs = 30; //Character size
const float csp = 45; //Character size + spacing

Results::Results(sf::Font const* font) {
	for (int i = 0; i < 2*nTotal; i++) {
		_kanas[i].setCharacterSize(cs);
		_kanas[i].setFillColor(sf::Color::Green);
		_kanas[i].setFont(*font);
		_kanas[i].setString(kana[i]);
		sf::Rect textBounds = _kanas[i].getLocalBounds();
		_kanas[i].setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height) / 2.f);
	}

	int index = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i == 1 || i == 3) && (j == 7 || j == 9)) {}
			else if (i == 2 && j == 9) {}
			else {
				_kanas[index].setPosition(cs/2 + j*csp, cs/2 + i*csp);
				index++;
			}
		}	
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			_kanas[index].setPosition(cs/2 + 10*csp + 80 + j*csp, cs/2 + i*csp);
			index++;
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i == 1 || i == 3) && (j == 7 || j == 9)) {}
			else if (i == 2 && j == 9) {}
			else {
				std::cout << index << std::endl;
				_kanas[index].setPosition(cs/2 + j*csp, cs/2 + 5*csp + 80 + i*csp);
				index++;
			}
		}	
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			_kanas[index].setPosition(cs/2 + 10*csp + 80 + j*csp, cs/2 + 5*csp + 80 + i*csp);
			index++;
		}
	}
}

sf::FloatRect Results::getLocalBounds() const {
	sf::FloatRect bounds;
	bounds.left = 0;
	bounds.top = 0;
	bounds.height = 10*csp + 80;
	bounds.width = 15*csp + 80;
	return bounds;
}

void Results::update(float* weights) {
	for (int i = 0; i < nTotal * 2; i++) {
		//Interpolate between green and red
		sf::Uint8 red;
		if (weights[i] <= 1.f) {
			red = (sf::Uint8)((weights[i] - .25f) / (1.f - .25f) * 127);
		}
		else {
			red = 127 + (sf::Uint8)((weights[i] - 1.f) / (10.f - 1.f) * 128);
		}
		_kanas[i].setFillColor(sf::Color(red, 255-red, 0));
	}
}

void Results::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	for (int i = 0; i < 2*nTotal; i++)
		target.draw(_kanas[i], states);
}
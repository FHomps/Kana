#include "Test.hpp"
#include "Kana.hpp"

const float mvtTime = 0.3f;

const sf::Vector2f Test::scale_normal{1.f, 1.f};
const sf::Vector2f Test::scale_highlighted{0.5f, 0.5f};
const sf::Color Test::color_normal = sf::Color(180, 180, 180);
const sf::Color Test::color_highlighted = sf::Color::White;
const float Test::spacing_normal = 100.f;
const float Test::spacing_highlighted = 200.f;

bool Test::_computedSpeeds = false;
float Test::mvSpeed_normal;
float Test::mvSpeed_highlighted;
sf::Vector2f Test::scaleSpeed;
sf::Vector3f Test::colorSpeed;

Test::Test(int index, sf::Font const* font) {
	_kana.setFont(*font);
	_kana.setString(kana[index]);
	if (_computedSpeeds) {
		computeSpeeds();
		_computedSpeeds = true;
	}
}

void Test::update() {
	if (_isHighlighting) {
		setScale(getScale() + scaleSpeed);
		_preciseColor += colorSpeed;

		if (getScale().x > scale_highlighted.x) {
			setScale(scale_highlighted);
			_isHighlighting = false;
			_isHighlighted = true;
		}
	}
	else if (_isDarkening) {

	}

	if (_isMoving) {
		if (_isHighlighting || _isDarkening || _isHighlighted) { //Move farther because the text is bigger

		}
		else {

		}
	}
}

void Test::computeSpeeds() {
	mvSpeed_normal = spacing_normal / mvtTime;
	mvSpeed_highlighted = spacing_highlighted / mvtTime;
	scaleSpeed = (scale_highlighted - scale_normal) / mvtTime;
	sf::Color colorDiff = color_highlighted - color_normal;
 	colorSpeed = sf::Vector3f((float)colorDiff.r / mvtTime,
							  (float)colorDiff.g / mvtTime,
						      (float)colorDiff.b / mvtTime);
}

void Test::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(_kana);
}
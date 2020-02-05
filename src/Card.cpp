#include "Card.hpp"
#include "Kana.hpp"

const float mvtTime = 60 * 0.1f;

const sf::Vector2f Card::scale_normal{0.5f, 0.5f};
const sf::Vector2f Card::scale_highlighted{1.f, 1.f};
const sf::Color Card::color_normal = sf::Color(180, 180, 180);
const sf::Color Card::color_highlighted = sf::Color::White;
const float Card::spacing_normal = 100.f;
const float Card::spacing_highlighted = 200.f;

bool Card::_computedSpeeds = false;
float Card::mvSpeed_normal;
float Card::mvSpeed_highlighted;
sf::Vector2f Card::scaleSpeed;
sf::Vector3f Card::colorSpeed;

Card::Card(int index, sf::Font const* font, bool highlighted) {
	_index = index;
	_kana.setFont(*font);
	_kana.setString(kana[index]);
	_kana.setCharacterSize(196);
	sf::Rect textBounds = _kana.getLocalBounds();
	_kana.setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height) / 2.f);
	_kana.setFillColor(highlighted ? color_highlighted : color_normal);
	_kana.setScale(highlighted ? scale_highlighted : scale_normal);
	_isHighlighted = highlighted;
	_isNormal = !highlighted;

	if (!_computedSpeeds) {
		computeSpeeds();
		_computedSpeeds = true;
	}
}

void Card::startMoving(bool toHighlight) {
	if (_isMoving)
		return;
	_isMoving = true;
	_initialPos = getPosition().x;

	if (toHighlight) {
		_isHighlighting = _isNormal;
	}
	else {
		_isDarkening = _isHighlighted;
	}
}

void Card::update() {
	if (_isMoving) {
		if (_isHighlighting) {
			_preciseColor += colorSpeed;
			
			_kana.setScale(_kana.getScale() + scaleSpeed);
			sf::Color newColor = sf::Color(_preciseColor.x, _preciseColor.y, _preciseColor.z);
			if (_kana.getFillColor() != newColor) {
				_kana.setFillColor(newColor);
			}
			_isNormal = false;
		}
		else if (_isDarkening) {
			_preciseColor -= colorSpeed;

			_kana.setScale(_kana.getScale() - scaleSpeed);
			sf::Color newColor = sf::Color(_preciseColor.x, _preciseColor.y, _preciseColor.z);
			if (_kana.getFillColor() != newColor) {
				_kana.setFillColor(newColor);
			}
			_isHighlighted = false;
		}

		if (!_isNormal) { //Move farther because the text is bigger
			if (getPosition().x < _initialPos - spacing_highlighted) {
				setPosition(_initialPos - spacing_highlighted, getPosition().y);
				_isMoving = false;
				
				if (_isHighlighting) {
					_kana.setScale(scale_highlighted);
					_preciseColor = sf::Vector3f(color_highlighted.r, color_highlighted.g, color_highlighted.b);
					_kana.setFillColor(color_highlighted);
					_isHighlighting = false;
					_isHighlighted = true;
				}
				else if (_isDarkening) {
					_kana.setScale(scale_normal);
					_preciseColor = sf::Vector3f(color_normal.r, color_normal.g, color_normal.b);
					_kana.setFillColor(color_normal);
					_isDarkening = false;
					_isNormal = true;
				}
			}
			else {
				move(-mvSpeed_highlighted, 0);
			}
		}
		else {
			if (getPosition().x < _initialPos - spacing_normal) {
				setPosition(_initialPos - spacing_normal, getPosition().y);
				_isMoving = false;
			}
			else {
				move(-mvSpeed_normal, 0);
			}
		}
	}
}

void Card::computeSpeeds() {
	mvSpeed_normal = spacing_normal / mvtTime;
	mvSpeed_highlighted = spacing_highlighted / mvtTime;
	scaleSpeed = (scale_highlighted - scale_normal) / mvtTime;
	sf::Color colorDiff = color_highlighted - color_normal;
 	colorSpeed = sf::Vector3f((float)colorDiff.r / mvtTime,
							  (float)colorDiff.g / mvtTime,
						      (float)colorDiff.b / mvtTime);
}

void Card::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(_kana, states);
}
#ifndef TEST_HPP
#define TEST_HPP

class Test : public sf::Drawable, public sf::Transformable {
public:
	Test(int kanaIndex, sf::Font const* font);

	int getIndex() const { return _index; }

	void update();

	void startMoveLeft() { _isMoving = true; }

	void startHighlighting() { if (_isHighlighted) return; _isHighlighting = true; _isDarkening = false; }

	void stopHighlighting() { if (!_isHighlighted) return; _isHighlighting = false; _isDarkening = true; }

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	static void computeSpeeds();

	int _index;

	sf::Text _kana;

	bool _isMoving = false;
	bool _isHighlighting = false;
	bool _isDarkening = false;

	bool _isHighlighted = false;

	//float-typed color mirror, necessary due to color change speed rounding to 0 if using bytes for rgb values
	sf::Vector3f _preciseColor = sf::Vector3f(color_normal.r, color_normal.g, color_normal.b);

	static const sf::Vector2f scale_normal;
	static const sf::Vector2f scale_highlighted;

	static const sf::Color color_normal;
	static const sf::Color color_highlighted;

	static const float spacing_normal;
	static const float spacing_highlighted;

	static bool _computedSpeeds;
	static float mvSpeed_normal;
	static float mvSpeed_highlighted;
	static sf::Vector2f scaleSpeed;
	static sf::Vector3f colorSpeed;
};

#endif
#pragma once
#include "Kana.hpp"

class Results : public sf::Drawable, public sf::Transformable {
public:

Results(sf::Font const& font);
void update(float* weights);

sf::FloatRect getLocalBounds() const;

private:
virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

sf::Text _kanas[2*nTotal];

};
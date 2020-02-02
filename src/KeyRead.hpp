#pragma once

sf::String keyToStr(const sf::Keyboard::Key& k){
	sf::String ret;

	switch(k){
    case sf::Keyboard::A :
        ret="a";
        break;

    case sf::Keyboard::B :
        ret="b";
        break;

    case sf::Keyboard::C :
        ret="c";
        break;

    case sf::Keyboard::D :
        ret="d";
        break;

    case sf::Keyboard::E :
        ret="e";
        break;

    case sf::Keyboard::F :
        ret="f";
        break;

    case sf::Keyboard::G :
        ret="g";
        break;
	
    case sf::Keyboard::H :
        ret="h";
        break;

    case sf::Keyboard::I :
        ret="i";
        break;

    case sf::Keyboard::J :
        ret="j";
        break;
	
    case sf::Keyboard::K :
        ret="k";
        break;

    case sf::Keyboard::L :
        ret="l";
        break;
	
    case sf::Keyboard::M :
        ret="m";
        break;

    case sf::Keyboard::N :
        ret="n";
        break;

    case sf::Keyboard::O :
        ret="o";
        break;

    case sf::Keyboard::P :
        ret="p";
        break;

    case sf::Keyboard::Q :
        ret="q";
        break;

    case sf::Keyboard::R :
        ret="r";
        break;

    case sf::Keyboard::S :
        ret="s";
        break;

    case sf::Keyboard::T :
        ret="t";
        break;
	
    case sf::Keyboard::U :
        ret="u";
        break;
	
    case sf::Keyboard::V :
        ret="v";
        break;
	
    case sf::Keyboard::W :
        ret="w";
        break;

    case sf::Keyboard::X :
        ret="x";
        break;

    case sf::Keyboard::Y :
        ret="y";
        break;

    case sf::Keyboard::Z :
        ret="z";
        break;
	
	default:
		ret="";
		break;
	}

	return ret;
}
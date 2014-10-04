#pragma once
#include <SFML\Graphics.hpp>

class Sundial
{
private:
	sf::Time time;
	sf::Sprite *frame, *dial;
	bool newDay;

	bool nightTime;

public:
	Sundial();

	sf::Sprite* getFrameSprite() const;
	sf::Sprite* getDialSprite() const;

	sf::Time getTime() const;

	bool isNewDay();

	bool isDayTime() const;

	void update(const sf::Time &dt);
};
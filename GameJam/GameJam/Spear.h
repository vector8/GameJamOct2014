#pragma once

#include <SFML\Graphics.hpp>

class Spear
{
private:
	const float FLIGHT_SPEED = 0.4f;
	const float RANGE = 200.f;
	const float WIDTH = 20;
	const float HEIGHT = 5;

	sf::Sprite* sprite;
	sf::Vector2f pos, dir;

	float interpolation;

	bool finished;

public:
	Spear(const sf::Vector2f &pos, const sf::Vector2f &goal, float scale);
	~Spear();

	sf::Sprite* getSprite() const;
	bool isFinished() const;

	void update(const sf::Time &dt);
};
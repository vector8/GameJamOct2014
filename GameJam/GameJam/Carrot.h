#pragma once
#include <SFML\Graphics.hpp>

class Carrot
{
private:
	sf::Sprite* sprite;

	bool finished;

public:
	Carrot(const sf::Vector2f &pos);
	~Carrot();

	sf::Sprite* getSprite() const;
	bool isFinished() const;

	void update(const sf::Time &dt);
};
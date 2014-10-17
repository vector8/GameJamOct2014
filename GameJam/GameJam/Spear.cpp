#include "Spear.h"
#include "Utilities.h"
#include "Globals.h"
#include "Game.h"
#include <iostream>

Spear::Spear(const sf::Vector2f &pos, const sf::Vector2f &goal, float scale) : sprite(new sf::Sprite()), pos(pos), interpolation(0.f), finished(false)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("images/spear.png"))
	{
		std::cout << "Error loading image file" << std::endl;
	}

	this->sprite->setTexture(*texture);
	this->sprite->setTextureRect(sf::IntRect(0, 0, WIDTH, HEIGHT));

	dir = v8g::normalize(goal - pos);

	this->sprite->setPosition(pos);
	this->sprite->setOrigin(WIDTH / 2, HEIGHT / 2);
	this->sprite->setScale(scale, scale);

	float theta = atan2f(dir.y, dir.x);

	this->sprite->setRotation(theta * 180 / PI);
}

Spear::~Spear()
{
	if (sprite)
	{
		delete sprite;
		sprite = NULL;
	}
}

sf::Sprite* Spear::getSprite() const
{
	return this->sprite;
}

bool Spear::isFinished() const
{
	return finished;
}

void Spear::update(const sf::Time &dt)
{
	float dx = 0.f, dy = 0.f;

	dx = FLIGHT_SPEED * dir.x;
	dy = FLIGHT_SPEED * dir.y;
	
	sf::Vector2f oldPos = this->sprite->getPosition();

	sf::Vector2f newPos(oldPos.x + dx * dt.asMilliseconds(), oldPos.y + dy * dt.asMilliseconds());

	if (v8g::length(newPos - pos) >= RANGE)
	{
		newPos.x = pos.x + dir.x * RANGE;
		newPos.y = pos.y + dir.y * RANGE;
		finished = true;
	}

	sprite->setPosition(newPos);

	if (v8g::length(newPos - Game::getGame()->getPlayer()->getSprite()->getPosition()) < (SPRITE_SIZE/2) * Game::getGame()->getPlayer()->getSprite()->getScale().y)
	{
		Game::getGame()->setGameOver();
		finished = true;
	}
}
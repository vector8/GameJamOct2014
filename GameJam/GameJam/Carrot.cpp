#include "Carrot.h"
#include "Globals.h"
#include "Utilities.h"
#include "Game.h"
#include <iostream>

Carrot::Carrot(const sf::Vector2f &pos) : sprite(new sf::Sprite()), finished(false)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("images/carrot.png"))
	{
		std::cout << "Error loading image file" << std::endl;
	}

	this->sprite->setTexture(*texture);
	this->sprite->setTextureRect(sf::IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));

	this->sprite->setPosition(pos);
	this->sprite->setOrigin(SPRITE_SIZE / 2, SPRITE_SIZE / 2);
}

Carrot::~Carrot()
{
	if (sprite)
	{
		delete sprite;
		sprite = NULL;
	}
}

sf::Sprite* Carrot::getSprite() const
{
	return this->sprite;
}

bool Carrot::isFinished() const
{
	return finished;
}

void Carrot::update(const sf::Time &dt)
{
	if (v8g::length(sprite->getPosition() - Game::getGame()->getPlayer()->getSprite()->getPosition()) < (SPRITE_SIZE / 2) * Game::getGame()->getPlayer()->getSprite()->getScale().y)
	{
		Game::getGame()->nomCarrot();
		finished = true;
	}
}
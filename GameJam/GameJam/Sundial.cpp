#include "Sundial.h"
#include <iostream>
#include "Globals.h"

Sundial::Sundial() : time(sf::Time::Zero), newDay(false), nightTime(false)
{
	this->dial = new sf::Sprite();
	this->frame = new sf::Sprite();

	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("images/sundial_dial.png"))
	{
		std::cout << "Error loading image file" << std::endl;
	}

	this->dial->setTexture(*texture);
	this->dial->setTextureRect(sf::IntRect(0, 0, 64, 64));
	this->dial->setOrigin(32, 32);
	this->dial->setPosition(WINDOW_WIDTH - 74, 74);
	this->dial->setScale(1.98f, 1.98f);

	texture = new sf::Texture();
	if (!texture->loadFromFile("images/sundial_frame.png"))
	{
		std::cout << "Error loading image file" << std::endl;
	}

	this->frame->setTexture(*texture);
	this->frame->setTextureRect(sf::IntRect(0, 0, 64, 64));
	this->frame->setOrigin(32, 32);
	this->frame->setPosition(WINDOW_WIDTH - 74, 74);
	this->frame->setScale(2.f, 2.f);
}

sf::Sprite* Sundial::getFrameSprite() const
{
	return this->frame;
}

sf::Sprite* Sundial::getDialSprite() const
{
	return this->dial;
}

sf::Time Sundial::getTime() const
{
	return this->time;
}

bool Sundial::isNewDay()
{
	bool result = newDay;
	newDay = false;
	return result;
}

bool Sundial::isDayTime() const
{
	return !nightTime;
}

void Sundial::update(const sf::Time &dt)
{
	this->time += dt;

	if (time.asSeconds() > DAY_LENGTH)
	{
		time -= sf::seconds(DAY_LENGTH);
	}

	if (time.asSeconds() > 0.f && time.asSeconds() <= DAY_LENGTH / 2.f && nightTime)
	{
		nightTime = false;
		newDay = true;
	}
	else if (time.asSeconds() > DAY_LENGTH / 2.f && time.asSeconds() <= DAY_LENGTH && !nightTime)
	{
		nightTime = true;
		newDay = true;
	}

	this->dial->setRotation(-(this->time.asSeconds() / DAY_LENGTH) * 360.f);
}

void Sundial::reset()
{
	time = sf::Time::Zero;
	newDay = false;
	nightTime = false;
}
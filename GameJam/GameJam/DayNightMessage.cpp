#include "DayNightMessage.h"
#include "Globals.h"
#include "Utilities.h"
#include <iostream>

DayNightMessage::DayNightMessage() : day(1), daytime(true), alphaInterpolation(0.f), currentState(MessageState::FADE_IN), displayTimer(sf::Time::Zero)
{
	message = new sf::Text();

	sf::Font *arialFont = new sf::Font();
	if (!arialFont->loadFromFile("fonts/arial.ttf"))
	{
		std::cout << "Error loading font file" << std::endl;
	}

	message->setFont(*arialFont);
	message->setCharacterSize(30);
	message->setStyle(sf::Text::Bold);
	message->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3);
}

sf::Text DayNightMessage::getMessage() const
{
	return *this->message;
}

void DayNightMessage::nextDay()
{
	day++;
	daytime = true;
	currentState = MessageState::FADE_IN;
}

void DayNightMessage::nextNight()
{
	daytime = false;
	currentState = MessageState::FADE_IN;
}

void DayNightMessage::update(const sf::Time &dt)
{
	if (currentState != MessageState::INVISIBLE)
	{
		if (daytime)
		{
			message->setString("Day " + v8g::intToString(day) + " - Run!");
			message->setColor(sf::Color::Blue);
		}
		else
		{
			message->setString("Night " + v8g::intToString(day) + " - Hunt!");
			message->setColor(sf::Color::Red);
		}
	}

	sf::Uint8 prevAlpha, nextAlpha;
	if (currentState == MessageState::FADE_IN || currentState == MessageState::FADE_OUT)
	{
		if (currentState == MessageState::FADE_IN)
		{
			prevAlpha = 0;
			nextAlpha = 255;
		}
		else
		{
			prevAlpha = 255;
			nextAlpha = 0;
		}

		alphaInterpolation += FADE_SPEED * dt.asMilliseconds();

		if (alphaInterpolation > 1.f)
			alphaInterpolation = 1.f;

		sf::Color currColor = message->getColor();

		message->setColor(sf::Color(currColor.r, currColor.g, currColor.b, (sf::Uint8)v8g::lerp(alphaInterpolation, prevAlpha, nextAlpha)));

		if (v8g::compareFloats(alphaInterpolation, 1.f))
		{
			if (currentState == MessageState::FADE_IN)
			{
				currentState = MessageState::DISPLAY;
				displayTimer = sf::seconds(DISPLAY_DURATION);
			}
			else
				currentState = MessageState::INVISIBLE;

			alphaInterpolation = 0.f;
		}
	}
	else if (currentState == MessageState::DISPLAY)
	{
		displayTimer -= dt;

		if (displayTimer <= sf::Time::Zero)
			currentState = MessageState::FADE_OUT;
	}

	sf::FloatRect msgRect = message->getLocalBounds();
	message->setOrigin(msgRect.width / 2, msgRect.height / 2);
}

void DayNightMessage::reset()
{
	day = 1;
	daytime = true;
	alphaInterpolation = 0.f;
	currentState = MessageState::FADE_IN;
	displayTimer = sf::Time::Zero;
}
#pragma once
#include <string>
#include <SFML\Graphics.hpp>

const float FADE_SPEED = 0.00255f;
const float DISPLAY_DURATION = 2.f;

enum class MessageState
{
	FADE_IN,
	DISPLAY,
	FADE_OUT,
	INVISIBLE
};

class DayNightMessage
{
private:
	sf::Text *message;
	int day;
	bool displaying, daytime;
	float alphaInterpolation;
	MessageState currentState;
	sf::Time displayTimer;

public:
	DayNightMessage();

	sf::Text getMessage() const;

	void nextDay();
	void nextNight();

	void update(const sf::Time &dt);
};
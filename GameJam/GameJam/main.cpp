#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Player.h"
#include "Sundial.h"
#include "Utilities.h"
#include "DayNightMessage.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game Jam");

	window.setFramerateLimit(60);

	Player p;
	Sundial s;
	DayNightMessage m;

	sf::Clock clock;
	sf::Time elapsed(sf::Time::Zero);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::Space:
					p.jump();
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		if (s.isNewDay())
		{
			p.mutate();

			if (s.isDayTime())
				m.nextDay();
			else
				m.nextNight();
		}

		elapsed = clock.restart();

		p.update(elapsed);
		s.update(elapsed);
		m.update(elapsed);

		float time = s.getTime().asSeconds() - DAY_LENGTH/4;
		if (time < 0.f)
			time += DAY_LENGTH;
		if (time > DAY_LENGTH/2)
			time = DAY_LENGTH - time;

		time = time / (DAY_LENGTH/2);

		sf::Uint8 green = (sf::Uint8)(v8g::lerp(time, 0.8f, 0.3f) * 255.f);
		sf::Uint8 blue = (sf::Uint8)(v8g::lerp(time, 0.f, 0.3f) * 255.f);
		window.clear(sf::Color(0, green, blue));
		window.draw(*p.getSprite());
		window.draw(*s.getDialSprite());
		window.draw(*s.getFrameSprite());
		window.draw(m.getMessage());
		window.display();
	}

	return 0;
}
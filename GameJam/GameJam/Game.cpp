#include "Game.h"
#include "Globals.h"
#include "Utilities.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Periodic Predator"), p(new Player()), s(new Sundial()), m(new DayNightMessage()), elapsed(sf::Time::Zero),
enemySpawnTimer(sf::Time::Zero), currAvgSpawnTime(AVG_ENEMY_SPAWN_TIME), carrotSpawnTimer(sf::milliseconds(CARROT_SPAWN_TIME)), food(MAX_FOOD), carrotsNommed(0), huntersNommed(0),
foodBar(sf::Vector2f(FOOD_BAR_WIDTH, FOOD_BAR_HEIGHT)), foodBarBG(sf::Vector2f(FOOD_BAR_WIDTH, FOOD_BAR_HEIGHT)), foodBarFlashing(false), 
foodBarFlashTimer(sf::milliseconds(FOOD_BAR_FLASH_DURATION)), daysSurvived(1)
{
	gameOverText = new sf::Text();
	statsText = new sf::Text();

	sf::Font *arialFont = new sf::Font();
	if (!arialFont->loadFromFile("fonts/arial.ttf"))
	{
		std::cout << "Error loading font file" << std::endl;
	}

	gameOverText->setFont(*arialFont);
	gameOverText->setCharacterSize(30);
	gameOverText->setStyle(sf::Text::Bold);
	gameOverText->setColor(sf::Color::Red);
	gameOverText->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3);
	gameOverText->setString("GAME OVER - Press Enter to try again!");
	sf::FloatRect textRect = gameOverText->getLocalBounds();
	gameOverText->setOrigin(textRect.width / 2, textRect.height / 2);

	statsText->setFont(*arialFont);
	statsText->setCharacterSize(18);
	statsText->setColor(sf::Color::Blue);
	statsText->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	statsText->setString("Survived Until: Day 1\n\nCarrots Eaten: 0\n\nHunters Eaten: 0");
	textRect = statsText->getLocalBounds();
	statsText->setOrigin(textRect.width / 2, textRect.height / 2);

	window.setFramerateLimit(60);

	foodBar.setFillColor(sf::Color::Yellow);
	foodBarBG.setFillColor(sf::Color::White);
	foodBarBG.setOutlineThickness(2);
	foodBarBG.setOutlineColor(sf::Color::Yellow);

	foodBar.setPosition(45, 10);
	foodBarBG.setPosition(45, 10);

	apple = new sf::Sprite();

	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("images/apple.png"))
	{
		std::cout << "Error loading image file" << std::endl;
	}

	apple->setTexture(*texture);
	apple->setTextureRect(sf::IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));

	apple->setPosition(10, 10);
}

Player* Game::getPlayer() const
{
	return p;
}

void Game::setGameOver()
{
	isGameOver = true;
}

void Game::run()
{
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
					p->jump();
					break;
				case sf::Keyboard::Return:
					if (isGameOver)
					{
						isGameOver = false;
						enemies.clear();
						spears.clear();
						carrots.clear();
						p->reset();
						s->reset();
						m->reset();
						food = MAX_FOOD;
						carrotsNommed = 0;
						huntersNommed = 0;
						daysSurvived = 1;
						currAvgSpawnTime = AVG_ENEMY_SPAWN_TIME;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		if (s->isNewDay())
		{
			p->mutate();

			if (s->isDayTime())
			{
				m->nextDay();
				currAvgSpawnTime /= 1.5f;
				if (currAvgSpawnTime < 0.5f)
					currAvgSpawnTime = 0.5f;

				if (!isGameOver)
					daysSurvived++;
			}
			else
				m->nextNight();
		}

		elapsed = clock.restart();

		if (!isGameOver)
		{
			food -= FOOD_DEPLETION_RATE * elapsed.asMilliseconds();

			if (food <= 0.f)
			{
				food = 0.f;
				isGameOver = true;
			}
			else if (food < FOOD_AMOUNT_WARNING)
				foodBarFlashing = true;
			else
			{
				foodBarFlashing = false;
				foodBar.setFillColor(sf::Color::Yellow);
				foodBarBG.setOutlineColor(sf::Color::Yellow);
			}

			foodBar.setSize(sf::Vector2f((food / MAX_FOOD) * FOOD_BAR_WIDTH, FOOD_BAR_HEIGHT));
		}

		if (foodBarFlashing)
		{
			foodBarFlashTimer -= elapsed;

			if (foodBarFlashTimer <= sf::Time::Zero)
			{
				foodBarFlashTimer = sf::milliseconds(FOOD_BAR_FLASH_DURATION);

				if (foodBar.getFillColor() == sf::Color::Yellow)
				{
					foodBar.setFillColor(sf::Color::Red);
					foodBarBG.setOutlineColor(sf::Color::Red);
				}
				else
				{
					foodBar.setFillColor(sf::Color::Yellow);
					foodBarBG.setOutlineColor(sf::Color::Yellow);
				}
			}
		}

		enemySpawnTimer -= elapsed;
		if (enemySpawnTimer <= sf::Time::Zero)
		{
			// spawn an enemy with random location
			Enemy* e = new Enemy(v8g::rollNumber(0, WINDOW_WIDTH), v8g::rollNumber(0, WINDOW_HEIGHT));
			enemies.push_back(e);
			// set the timer to a random value with avg currAvgSpawnTimer
			enemySpawnTimer = sf::milliseconds(v8g::rollNumber(currAvgSpawnTime * 0.5, currAvgSpawnTime * 1.5, 4));
		}

		carrotSpawnTimer -= elapsed;
		if (carrotSpawnTimer <= sf::Time::Zero)
		{
			// spawn a carrot with random location
			Carrot* c = new Carrot(sf::Vector2f(v8g::rollNumber(0, WINDOW_WIDTH), v8g::rollNumber(0, WINDOW_HEIGHT)));
			carrots.push_back(c);
			carrotSpawnTimer = sf::milliseconds(CARROT_SPAWN_TIME);
		}

		if (!isGameOver)
			p->update(elapsed);
		s->update(elapsed);
		m->update(elapsed);

		for (std::vector<Spear*>::iterator it = spears.begin(); it != spears.end(); it++)
		{
			if (!(*it)->isFinished())
				(*it)->update(elapsed);
		}

		for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++)
		{
			if (!(*it)->isFinished())
			{
				if (isGameOver)
					(*it)->update(elapsed, sf::Vector2f(100000, 100000), s->isDayTime());
				else
					(*it)->update(elapsed, p->getSprite()->getPosition(), s->isDayTime());
			}
		}

		for (std::vector<Carrot*>::iterator it = carrots.begin(); it != carrots.end(); it++)
		{
			if (!(*it)->isFinished())
				(*it)->update(elapsed);
		}

		if (isGameOver)
		{
			std::string statsString = "Survived Until: Day " + v8g::intToString(daysSurvived) + "\n\nCarrots Eaten: " + v8g::intToString(carrotsNommed) + "\n\nHunters Eaten: " + v8g::intToString(huntersNommed);
			statsText->setString(statsString);
			sf::FloatRect textRect = statsText->getLocalBounds();
			statsText->setOrigin(textRect.width / 2, textRect.height / 2);
		}

		float time = s->getTime().asSeconds() - DAY_LENGTH / 4;
		if (time < 0.f)
			time += DAY_LENGTH;
		if (time > DAY_LENGTH / 2)
			time = DAY_LENGTH - time;

		time = time / (DAY_LENGTH / 2);

		sf::Uint8 green = (sf::Uint8)(v8g::lerp(time, 0.8f, 0.3f) * 255.f);
		sf::Uint8 blue = (sf::Uint8)(v8g::lerp(time, 0.f, 0.3f) * 255.f);
		window.clear(sf::Color(0, green, blue));
		for (std::vector<Carrot*>::iterator it = carrots.begin(); it != carrots.end(); it++)
			if (!(*it)->isFinished())
				window.draw(*(*it)->getSprite());
		if (!isGameOver)
			window.draw(*p->getSprite());
		for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++)
			if (!(*it)->isFinished())
				window.draw(*(*it)->getSprite());
		for (std::vector<Spear*>::iterator it = spears.begin(); it != spears.end(); it++)
			if (!(*it)->isFinished())
				window.draw(*(*it)->getSprite());
		window.draw(*s->getDialSprite());
		window.draw(*s->getFrameSprite());
		window.draw(*apple);
		window.draw(foodBarBG);
		window.draw(foodBar);
		if (!isGameOver)
			window.draw(m->getMessage());
		if (isGameOver)
		{
			window.draw(*gameOverText);
			window.draw(*statsText);
		}

		window.display();
	}
}

void Game::createSpear(const sf::Vector2f &pos, const sf::Vector2f &goal, float scale)
{
	Spear *s = new Spear(pos, goal, scale);
	spears.push_back(s);
}

void Game::nomCarrot()
{
	carrotsNommed++;
	food += FOOD_GAINED_PER_NOM;

	if (food > MAX_FOOD)
		food = MAX_FOOD;
}

void Game::nomHunter()
{
	huntersNommed++;
	food += FOOD_GAINED_PER_NOM;

	if (food > MAX_FOOD)
		food = MAX_FOOD;
}


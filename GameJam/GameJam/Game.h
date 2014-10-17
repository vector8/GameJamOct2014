#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Sundial.h"
#include "DayNightMessage.h"
#include "Enemy.h"
#include "Spear.h"
#include "Carrot.h"

class Game
{
private:
	const float AVG_ENEMY_SPAWN_TIME = 10000.f;
	const float CARROT_SPAWN_TIME = 4000.f;
	const float FOOD_DEPLETION_RATE = 0.005f;
	const float FOOD_GAINED_PER_NOM = 20.f;
	const float MAX_FOOD = 100.f;
	const float FOOD_BAR_WIDTH = 200.f;
	const float FOOD_BAR_HEIGHT = 32.f;
	const float FOOD_BAR_FLASH_DURATION = 500.f;
	const float FOOD_AMOUNT_WARNING = 50.f;

	Game();

	sf::RenderWindow window;

	Player* p;
	Sundial* s;
	DayNightMessage* m;

	sf::Text *gameOverText, *statsText;

	sf::Clock clock;
	sf::Time elapsed;
	sf::Time enemySpawnTimer, carrotSpawnTimer;
	sf::Time foodBarFlashTimer;
	float currAvgSpawnTime;
	bool isGameOver;
	float food;
	sf::RectangleShape foodBar, foodBarBG;
	bool foodBarFlashing;
	sf::Sprite *apple;

	int carrotsNommed, huntersNommed, daysSurvived;

	std::vector<Spear*> spears;
	std::vector<Enemy*> enemies;
	std::vector<Carrot*> carrots;

public:
	static Game* getGame()
	{
		static Game* g = new Game();

		return g;
	}

	Player* getPlayer() const;

	void setGameOver();

	void run();

	void createSpear(const sf::Vector2f &pos, const sf::Vector2f &goal, float scale);

	void nomCarrot();
	void nomHunter();
};
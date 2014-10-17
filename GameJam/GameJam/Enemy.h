#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

enum class EnemyAnimations
{
	IDLE,
	WALK,
	RUN,
	ATTACK,
	AGGRESSIVE
};

class Enemy
{
private:
	const float DETECTION_RADIUS = 200.f;
	const float MAX_CHASE_TIME = 1000.f;
	const int FRAME_MAX = 4;
	const float IDLE_FRAME_DURATION = 500.f;
	const float WALK_FRAME_DURATION = 200.f;
	const float RUN_FRAME_DURATION = 100.f;
	const float ATTACK_FRAME_DURATION = 100.f;
	const float RUN_SPEED = 0.2f;
	const float WALK_SPEED = 0.1f;
	const float MIN_WANDER_RANGE = 50.f;
	const float MAX_WANDER_RANGE = 200.f;
	const float ATTACK_COOLDOWN = 3000.f;
	const int MIN_IDLE_TIME = 1000;
	const int MAX_IDLE_TIME = 5000;
	const int SPRITE_SCALE = 2.f;

	sf::Sprite* sprite;
	sf::Time frameTimer, chaseTimer, idleTimer, attackTimer;
	int frame;
	EnemyAnimations anim;
	sf::Vector2f goal;
	bool finished;

	void setAnimation(EnemyAnimations a);

public:
	Enemy(float posX, float posY);
	~Enemy();

	sf::Sprite* getSprite() const;

	bool isFinished() const;

	void update(const sf::Time &dt, const sf::Vector2f &playerPos, bool daytime);
};
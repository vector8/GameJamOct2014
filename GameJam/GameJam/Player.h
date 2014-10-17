#pragma once
#include <SFML\Graphics.hpp>

enum class PlayerAnimations
{
	IDLE,
	RUN,
	DIVE
};

class Player
{
private:
	const int IDLE_FRAME_DURATION = 300;
	const int RUN_FRAME_DURATION = 100;
	const int JUMP_DURATION = 500;
	const int FRAME_MAX = 4;
	const float MOVE_SPEED = 0.2f;
	const float JUMP_SPEED = 0.5f;
	const float SCALE_SMALL = 2.f;
	const float SCALE_LARGE = 6.f;
	const float MUTATE_SPEED = 0.002f;

	sf::Sprite* sprite;
	sf::Time frameTimer;
	sf::Time jumpTimer;
	int frame;
	PlayerAnimations anim;
	sf::Vector2f currentVelocity;
	bool mutating;
	float prevScale, targetScale;
	float scaleInterpolation;

	void setAnimation(PlayerAnimations a);

public:
	Player();

	sf::Sprite* getSprite() const;

	void update(const sf::Time &dt);

	void jump();
	void mutate();

	void reset();
};
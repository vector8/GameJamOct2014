#include "Enemy.h"
#include <iostream>
#include "Globals.h"
#include "Utilities.h"
#include "Game.h"

Enemy::Enemy(float posX, float posY) : sprite(new sf::Sprite()), frameTimer(sf::Time::Zero), chaseTimer(sf::Time::Zero), attackTimer(sf::Time::Zero),
frame(0), anim(EnemyAnimations::IDLE), goal(0.f, 0.f), finished(false)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("images/enemy.png"))
	{
		std::cout << "Error loading image file" << std::endl;
	}

	this->sprite->setTexture(*texture);
	this->sprite->setTextureRect(sf::IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));

	this->sprite->setPosition(posX, posY);
	this->sprite->setScale(SPRITE_SCALE, SPRITE_SCALE);
	this->sprite->setOrigin(SPRITE_SIZE / 2, SPRITE_SIZE / 2);
}

Enemy::~Enemy()
{
	if (sprite)
	{
		delete sprite;
		sprite = NULL;
	}
}

void Enemy::setAnimation(EnemyAnimations a)
{
	this->anim = a;
	this->frameTimer = sf::Time::Zero;
	this->frame = 0;
}

sf::Sprite* Enemy::getSprite() const
{
	return this->sprite;
}

bool Enemy::isFinished() const
{
	return finished;
}

void Enemy::update(const sf::Time &dt, const sf::Vector2f &playerPos, bool daytime)
{
	if (attackTimer > sf::Time::Zero)
		attackTimer -= dt;

	// FRAME
	this->frameTimer += dt;

	float frameDuration;
	if (this->anim == EnemyAnimations::IDLE)
		frameDuration = IDLE_FRAME_DURATION;
	else if (this->anim == EnemyAnimations::WALK)
		frameDuration = WALK_FRAME_DURATION;
	else if (this->anim == EnemyAnimations::RUN)
		frameDuration = RUN_FRAME_DURATION;
	else if (this->anim == EnemyAnimations::ATTACK)
		frameDuration = ATTACK_FRAME_DURATION;

	if (this->frameTimer.asMilliseconds() > frameDuration)
	{
		this->frameTimer = sf::Time::Zero;
		this->frame++;

		if (frame >= FRAME_MAX)
		{
			if (this->anim == EnemyAnimations::ATTACK)
			{
				this->setAnimation(EnemyAnimations::RUN);
				Game::getGame()->createSpear(this->sprite->getPosition(), playerPos, SPRITE_SCALE);
			}
			else
				this->frame = this->frame % FRAME_MAX;
		}
	}

	// POSITION
	
	if (this->anim != EnemyAnimations::ATTACK)
	{
		float dx = 0.f, dy = 0.f;

		// chase / flee player if they are within detection radius OR chase timer is not up
		if (v8g::length(playerPos - this->sprite->getPosition()) < DETECTION_RADIUS)
		{
			if (attackTimer <= sf::Time::Zero && daytime)
			{
				this->setAnimation(EnemyAnimations::ATTACK);
				attackTimer = sf::milliseconds(ATTACK_COOLDOWN);
			}
			else
			{
				if (this->anim != EnemyAnimations::RUN)
					this->setAnimation(EnemyAnimations::RUN);

				this->chaseTimer = sf::milliseconds(MAX_CHASE_TIME);

				if (daytime)
					this->goal = playerPos;
				else
					this->goal = this->sprite->getPosition() - (playerPos - this->sprite->getPosition());

				sf::Vector2f dir = v8g::normalize(this->goal - this->sprite->getPosition());
				dx = dir.x * RUN_SPEED;
				dy = dir.y * RUN_SPEED;
			}
		}
		else if (this->chaseTimer > sf::Time::Zero)
		{
			this->chaseTimer -= dt;

			if (this->chaseTimer <= sf::Time::Zero)
				this->setAnimation(EnemyAnimations::IDLE);
			else if (this->anim != EnemyAnimations::RUN)
				this->setAnimation(EnemyAnimations::RUN);

			if (daytime)
				this->goal = playerPos;
			else
				this->goal = this->sprite->getPosition() - (playerPos - this->sprite->getPosition());

			sf::Vector2f dir = v8g::normalize(this->goal - this->sprite->getPosition());
			dx = dir.x * RUN_SPEED;
			dy = dir.y * RUN_SPEED;
		}
		else if (anim == EnemyAnimations::WALK)
		{
			sf::Vector2f dir = this->goal - this->sprite->getPosition();

			if (v8g::length(dir) > 5.f)
			{
				// advance toward current goal
				dir = v8g::normalize(dir);
				dx = dir.x * WALK_SPEED;
				dy = dir.y * WALK_SPEED;
			}
			else
			{
				this->setAnimation(EnemyAnimations::IDLE);
				idleTimer = sf::milliseconds(v8g::rollNumber(MIN_IDLE_TIME, MAX_IDLE_TIME));
			}
		}
		else
		{
			idleTimer -= dt;

			if (idleTimer <= sf::Time::Zero)
			{
				// find a new goal to wander to
				// find a random point within wander range (higher chance to move toward center of map the further out this is)
				float wanderDistance = v8g::rollNumber(MIN_WANDER_RANGE, MAX_WANDER_RANGE, 2);

				sf::Vector2f pos = this->sprite->getPosition();
				if (pos.x > WINDOW_WIDTH / 2)
					pos.x -= WINDOW_WIDTH / 2;
				if (pos.y > WINDOW_HEIGHT / 2)
					pos.y -= WINDOW_HEIGHT / 2;

				float chanceWest = pos.x / (WINDOW_WIDTH / 2);
				float chanceNorth = pos.y / (WINDOW_HEIGHT / 2);

				int rollX = v8g::rollNumber(0, 100);
				int rollY = v8g::rollNumber(0, 100);

				float dirX, dirY;

				if (rollX < chanceWest * 100.f)
					dirX = -1.f;
				else
					dirX = 1.f;

				if (rollY < chanceNorth * 100.f)
					dirY = -1.f;
				else
					dirY = 1.f;

				dirX *= v8g::rollNumber(0.f, 1.f, 3);
				dirY *= v8g::rollNumber(0.f, 1.f, 3);

				sf::Vector2f dir(dirX, dirY);
				dir = v8g::normalize(dir);

				this->goal = this->sprite->getPosition() + dir * wanderDistance;
				if (goal.x > WINDOW_WIDTH - SPRITE_SIZE / 2)
					goal.x = WINDOW_WIDTH - SPRITE_SIZE / 2;
				else if (goal.x < 0.f + SPRITE_SIZE / 2)
					goal.x = 0.f + SPRITE_SIZE / 2;

				if (goal.y > WINDOW_HEIGHT - SPRITE_SIZE / 2)
					goal.y = WINDOW_HEIGHT - SPRITE_SIZE / 2;
				else if (goal.y < 0.f + SPRITE_SIZE / 2)
					goal.y = 0.f + SPRITE_SIZE / 2;

				this->setAnimation(EnemyAnimations::WALK);
			}
		}

		sf::Vector2f pos(this->sprite->getPosition().x + dx * dt.asMilliseconds(), this->sprite->getPosition().y + dy * dt.asMilliseconds());

		if (pos.x > WINDOW_WIDTH - SPRITE_SIZE / 2)
			pos.x = WINDOW_WIDTH - SPRITE_SIZE / 2;
		else if (pos.x < 0.f + SPRITE_SIZE / 2)
			pos.x = 0.f + SPRITE_SIZE / 2;

		if (pos.y > WINDOW_HEIGHT - SPRITE_SIZE / 2)
			pos.y = WINDOW_HEIGHT - SPRITE_SIZE / 2;
		else if (pos.y < 0.f + SPRITE_SIZE / 2)
			pos.y = 0.f + SPRITE_SIZE / 2;

		this->sprite->setPosition(pos);

		// SCALE
		if (dx < 0)
			this->sprite->setScale(-abs(this->sprite->getScale().x), abs(this->sprite->getScale().y));
		else if (dx > 0)
			this->sprite->setScale(abs(this->sprite->getScale().x), abs(this->sprite->getScale().y));
	}

	this->sprite->setTextureRect(sf::IntRect(this->frame * SPRITE_SIZE, static_cast<int>(this->anim) * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));

	if (v8g::length(this->sprite->getPosition() - Game::getGame()->getPlayer()->getSprite()->getPosition()) < (SPRITE_SIZE/2) * Game::getGame()->getPlayer()->getSprite()->getScale().y)
	{
		if (daytime)
			Game::getGame()->setGameOver();
		else
		{
			Game::getGame()->nomHunter();
			finished = true;
		}
	}
}
#include "Player.h"
#include <iostream>
#include "Utilities.h"
#include "Globals.h"

Player::Player() : frame(0), anim(PlayerAnimations::IDLE), frameTimer(sf::Time::Zero), jumpTimer(sf::Time::Zero), 
mutating(false), prevScale(SCALE_SMALL), targetScale(SCALE_SMALL), scaleInterpolation(0.f)
{
	this->sprite = new sf::Sprite();

	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("images/player.png"))
	{
		std::cout << "Error loading image file" << std::endl;
	}

	this->sprite->setTexture(*texture);
	this->sprite->setTextureRect(sf::IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));

	this->sprite->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	this->sprite->setOrigin(SPRITE_SIZE / 2, SPRITE_SIZE/2);
	this->sprite->setScale(SCALE_SMALL, SCALE_SMALL);

	this->currentVelocity = sf::Vector2f(0.f, 0.f);
}

void Player::setAnimation(PlayerAnimations a)
{
	this->anim = a;
	this->frameTimer = sf::Time::Zero;
	this->frame = 0;
}

sf::Sprite* Player::getSprite() const
{
	return this->sprite;
}

void Player::update(const sf::Time &dt)
{
	// FRAME

	this->frameTimer += dt;

	float frameDuration;
	if (this->anim == PlayerAnimations::IDLE)
		frameDuration = IDLE_FRAME_DURATION;
	else if (this->anim == PlayerAnimations::RUN)
		frameDuration = RUN_FRAME_DURATION;
	else
		frameDuration = (float)JUMP_DURATION / (float)FRAME_MAX;

	if (this->frameTimer.asMilliseconds() > frameDuration)
	{
		this->frameTimer = sf::Time::Zero;
		this->frame++;
		this->frame = this->frame % FRAME_MAX;
	}

	// MOVEMENT
	float dx = 0.f, dy = 0.f;
	if (this->jumpTimer > sf::Time::Zero)
	{
		this->jumpTimer -= dt;
		if (this->jumpTimer < sf::Time::Zero)
			this->jumpTimer = sf::Time::Zero;

		dx = currentVelocity.x * JUMP_SPEED;
		dy = currentVelocity.y * JUMP_SPEED;
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			dy -= MOVE_SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			dy += MOVE_SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			dx += MOVE_SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			dx -= MOVE_SPEED;

		if (this->anim != PlayerAnimations::RUN && (!v8g::compareFloats(dx, 0.f) || !v8g::compareFloats(dy, 0.f)))
			this->setAnimation(PlayerAnimations::RUN);
		else if (this->anim != PlayerAnimations::IDLE && v8g::compareFloats(dx, 0.f) && v8g::compareFloats(dy, 0.f))
			this->setAnimation(PlayerAnimations::IDLE);

		currentVelocity.x = dx;
		currentVelocity.y = dy;

		currentVelocity = v8g::normalize(currentVelocity);
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

	if (mutating)
	{
		scaleInterpolation += MUTATE_SPEED * dt.asMilliseconds();
		if (scaleInterpolation > 1.f)
			scaleInterpolation = 1.f;
		float scale = v8g::lerp(scaleInterpolation, prevScale, targetScale);
		this->sprite->setScale(scale, abs(scale));

		if (v8g::compareFloats(scaleInterpolation, 1.f))
			mutating = false;
	}

	int offset = 0;
	if (!v8g::compareFloats(abs(this->sprite->getScale().x), SCALE_SMALL))
		offset = 96;

	this->sprite->setTextureRect(sf::IntRect(this->frame * SPRITE_SIZE, static_cast<int>(this->anim) * SPRITE_SIZE + offset, SPRITE_SIZE, SPRITE_SIZE));
}

void Player::jump()
{
	if (this->jumpTimer == sf::Time::Zero && (!v8g::compareFloats(currentVelocity.x, 0.f) || !v8g::compareFloats(currentVelocity.y, 0.f)))
	{
		this->jumpTimer = sf::milliseconds(JUMP_DURATION);
		this->setAnimation(PlayerAnimations::DIVE);
	}
}

void Player::mutate()
{
	float xScale = this->sprite->getScale().x;
	
	mutating = true;
	scaleInterpolation = 0.f;
	prevScale = xScale;

	if (v8g::compareFloats(abs(xScale), SCALE_SMALL))
		targetScale = (xScale / abs(xScale)) * SCALE_LARGE;
		//this->sprite->setScale((xScale / abs(xScale)) * SCALE_LARGE, SCALE_LARGE);
	else
		targetScale = (xScale / abs(xScale)) * SCALE_SMALL;
		//this->sprite->setScale((xScale / abs(xScale)) * SCALE_SMALL, SCALE_SMALL);
}
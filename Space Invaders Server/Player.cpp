#include <SFML/Graphics.hpp>
#include "Player.h"
#include <mutex>


Player::Player(float radius) : sf::CircleShape(radius) {
	this->xVelocity = 0;
	this->yVelocity = 0;
	this->canJump = true;
	this->isInAir = false;
	this->health = 100;
	this->canCollide = true;
	this->handler = new PlayerHandler(this);
	guid = 0;
	paused = false;
	scale = 1;
}

bool Player::doesCollide() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->canCollide;
}

double Player::getYVelocity() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->yVelocity;
}

void Player::setYVelocity(double velocity) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (velocity < -6500) {
		velocity = -6500;
	}
	this->yVelocity = velocity;
}

int Player::getHealth() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->health;
}

void Player::setCollides(bool setCollides) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->canCollide = setCollides;
}

void Player::setHealth(int health) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->health = health;
}

void Player::damage(int damage) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->health -= damage;
}

bool Player::isAirborne() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->isInAir;
}

void Player::setAirborne(bool status) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->isInAir = status;
}

void Player::setXVelocity(double xVelocity) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->xVelocity = xVelocity;
}

double Player::getXVelocity() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->xVelocity;
}

EventHandler* Player::getEventHandler() {
	return this->handler;
}

std::string Player::to_string() {
	std::string base = "";
	base.append(std::to_string(this->getPosition().x).append(" "));
	base.append(std::to_string(this->getPosition().y).append(" "));
	base.append(std::to_string(xVelocity).append(" "));
	base.append(std::to_string(yVelocity).append(" "));
	base.append(canJump ? "1 " : "0 ");
	base.append(isInAir ? "1 " : "0 ");
	sf::Color playerColor = this->getFillColor();
	base.append(std::to_string(playerColor.r).append(" "));
	base.append(std::to_string(playerColor.g).append(" "));
	base.append(std::to_string(playerColor.b).append(" "));
	base.append(std::to_string(health));
	return base;
}

std::string Player::to_string(sf::Vector2f spoofPosition) {
	std::string base = "";
	base.append(std::to_string(spoofPosition.x).append(" "));
	base.append(std::to_string(spoofPosition.y).append(" "));
	base.append(std::to_string(xVelocity).append(" "));
	base.append(std::to_string(yVelocity).append(" "));
	base.append(canJump ? "1 " : "0 ");
	base.append(isInAir ? "1 " : "0 ");
	sf::Color playerColor = this->getFillColor();
	base.append(std::to_string(playerColor.r).append(" "));
	base.append(std::to_string(playerColor.g).append(" "));
	base.append(std::to_string(playerColor.b).append(" "));
	base.append(std::to_string(health));
	return base;
}
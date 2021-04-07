#include "MovingPlatform.h"
#include <mutex>

MovingPlatform::MovingPlatform(sf::Vector2f vector) : sf::RectangleShape(vector) {
	canCollide = true;
	velocity = 0;
	isMovingRight = true;
}

bool MovingPlatform::doesCollide() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->canCollide;
}

double MovingPlatform::getVelocity() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->velocity;
}

void MovingPlatform::setCollides(bool canCollide) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->canCollide = canCollide;
}

void MovingPlatform::setVelocity(double velocity) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->velocity = velocity;
}

void MovingPlatform::setIsMovingRight(bool isMovingRight) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->isMovingRight = isMovingRight;
}

bool MovingPlatform::getIsMovingRight() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->isMovingRight;
}

std::string MovingPlatform::to_string() {
	std::string base = "";
	base.append(std::to_string(this->getPosition().x).append(" "));
	base.append(std::to_string(this->getPosition().y).append(" "));
	base.append(std::to_string(this->getSize().x).append(" "));
	base.append(std::to_string(this->getSize().y).append(" "));
	base.append(isMovingRight ? "1 " : "0 ");
	base.append(std::to_string(velocity));
	sf::Color platformColor = this->getFillColor();
	base.append(std::to_string(platformColor.r).append(" "));
	base.append(std::to_string(platformColor.g).append(" "));
	base.append(std::to_string(platformColor.b));
	return base;
}
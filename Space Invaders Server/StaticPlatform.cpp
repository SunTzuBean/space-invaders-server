#include <SFML/Graphics.hpp>
#include <mutex>
#include "StaticPlatform.h"

StaticPlatform::StaticPlatform(sf::Vector2f vector) : sf::RectangleShape(vector) {
	this->canCollide = true;
}

bool StaticPlatform::doesCollide() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return this->canCollide;
}

void StaticPlatform::setCollides(bool canCollide) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->canCollide = canCollide;
}

std::string StaticPlatform::to_string() {
	std::string base = "";
	base.append(std::to_string(this->getPosition().x).append(" "));
	base.append(std::to_string(this->getPosition().y).append(" "));
	base.append(std::to_string(this->getSize().x).append(" "));
	base.append(std::to_string(this->getSize().y).append(" "));
	sf::Color platformColor = this->getFillColor();
	base.append(std::to_string(platformColor.r).append(" "));
	base.append(std::to_string(platformColor.g).append(" "));
	base.append(std::to_string(platformColor.b).append(" "));
	return base;
}
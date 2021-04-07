#include "GameObject.h"

GameObject::GameObject() {
	this->shape = NULL;
	this->handler = NULL;
	guid = 0;
	viewOffset = 0;
}

sf::RectangleShape* GameObject::getShape() {
	return shape;
}

EventHandler* GameObject::getHandler() {
	return handler;
}

Platform::Platform(sf::RectangleShape* shape, bool isVisible, double xVelocity, double yVelocity, double period, std::string texture) {
	this->shape = shape;
	this->isVisible = isVisible;
	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;
	this->period = period;
	this->texture = texture;
	this->timePassed = 0;
	setTexture(texture);
	this->handler = new PlatformHandler(this);
}

bool Platform::getIsVisible() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return isVisible;
}

void Platform::setIsVisible(bool isVisible) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->isVisible = isVisible;
}

void Platform::move(double deltaT) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	shape->move(sf::Vector2f(xVelocity * deltaT, yVelocity * deltaT));
	if (timePassed > period / 2) {
		xVelocity = -xVelocity;
		yVelocity = -yVelocity;
		timePassed = 0;
	}
	timePassed += deltaT;
}

double Platform::getXVelocity() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return xVelocity;
}

double Platform::getYVelocity() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	return yVelocity;
}

void Platform::setXVelocity(double xVelocity) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->xVelocity = xVelocity;
}

void Platform::setYVelocity(double yVelocity) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	this->yVelocity = yVelocity;
}

void Platform::setTexture(std::string tex) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	sf::Texture t;
	if (!t.loadFromFile(tex)) {}
	t.setRepeated(true);
	this->shape->setTexture(&t);
}

std::string Platform::to_string() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	std::string base = "";
	base.append(std::to_string(this->shape->getPosition().x).append(" "));
	base.append(std::to_string(this->shape->getPosition().y).append(" "));
	base.append(std::to_string(this->shape->getSize().x).append(" "));
	base.append(std::to_string(this->shape->getSize().y).append(" "));
	base.append(std::to_string(this->xVelocity).append(" "));
	base.append(std::to_string(this->yVelocity).append(" "));
	sf::Color platformColor = this->shape->getFillColor();
	base.append(std::to_string(platformColor.r).append(" "));
	base.append(std::to_string(platformColor.g).append(" "));
	base.append(std::to_string(platformColor.b).append(" "));
	base.append(isVisible ? "1 " : "0 ");
	base.append(texture);
	return base;
}

Spawn::Spawn(float xPos, float yPos) {
	sf::RectangleShape* r = new sf::RectangleShape(sf::Vector2f(1, 1));
	r->setPosition(sf::Vector2f(xPos, yPos));
	shape = r;
	handler = new SpawnHandler(this);
}

void Spawn::respawn(Player* player) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	player->setPosition(shape->getPosition());
}

std::string Spawn::to_string() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	std::string base = "";
	base.append(std::to_string(this->shape->getPosition().x).append(" "));
	base.append(std::to_string(this->shape->getPosition().y));
	return base;
}

Boundary::Boundary(float width, float height, float xPos, float yPos, int spawnIndex) {
	sf::RectangleShape* r = new sf::RectangleShape(sf::Vector2f(width, height));
	r->setPosition(sf::Vector2f(xPos, yPos));
	this->spawnIndex = spawnIndex;
	shape = r;
	handler = new BoundaryHandler(this);
}

int Boundary::getSpawnIndex() {
	return spawnIndex;
}

std::string Boundary::to_string() {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	std::string base = "";
	base.append(std::to_string(this->shape->getPosition().x).append(" "));
	base.append(std::to_string(this->shape->getPosition().y).append(" "));
	base.append(std::to_string(this->shape->getSize().x).append(" "));
	base.append(std::to_string(this->shape->getSize().y).append(" "));
	base.append(std::to_string(spawnIndex));
	return base;
}
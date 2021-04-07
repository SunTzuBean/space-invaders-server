#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include "EventHandler.h"
class EventHandler;


class Player : public sf::CircleShape {
private:
	
	double xVelocity;
	double yVelocity;
	bool canJump;
	bool isInAir;
	int health;
	bool canCollide;
	EventHandler* handler;

public:
	int guid;
	bool paused;
	double scale;

	/**
	*Constructs a Player object, inherited from CircleShape.
	*@param radius the radius of the player.
	*/
	Player(float radius);

	/**
	*Tells whether or not it will collide (Not sure when to use)
	*/
	bool doesCollide();

	/**
	* Gets the y velocity of the player.
	* @return the y velocity of the player.
	*/
	double getYVelocity();

	/**
	* Gets the health of the player.
	* @return the health of the player.
	*/
	int getHealth();

	/**
	* Sets the collision (still don't know when to use.)
	* @param canCollide set canCollide.
	*/
	void setCollides(bool canCollide);

	/**
	* Sets the y velocity of the player.
	* @param the velocity of the player.
	*/
	void setYVelocity(double velocity);

	/**
	* Sets the health of the player to the given value.
	* @param health the health to set.
	*/
	void setHealth(int health);

	/**
	* Damages the player
	* @param damage the damage to deal to the player.
	*/
	void damage(int damage);

	/**
	* Checks if the player is airborne.
	* @return if the player is airborne
	*/
	bool isAirborne();

	/**
	* Sets whether or not this player will be airborne.
	* @param status whether or not to make the player airborne.
	*/
	void setAirborne(bool status);

	/**
	* Sets the x velocity for this player.
	* @param xVelocity the x velocity to give this player.
	*/
	void setXVelocity(double xVelocity);

	/**
	* Gets the x velocity for this player.
	* @return the x velocity of this player.
	*/
	double getXVelocity();

	EventHandler* getEventHandler();

	/**
	* Gets the string for this object.
	* @return the string representation of this object.
	*		  Order is:
	*		  x position: a float representing the X position of the player
	*		  y position: a float representing the Y position of the player
	*		  xVelocity: a double representing x velocity
	*		  yVelocity: a double representing y velocity
	*		  canJump: 0 or 1
	*		  isInAir: 0 or 1
	*		  r: red component of player
	*		  g: green component of player
	*		  b: blue component of player
	*		  health: an integer representing health
	*
	*/
	std::string to_string();

	std::string to_string(sf::Vector2f spoofPosition);

};
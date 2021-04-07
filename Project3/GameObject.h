#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include "Player.h"
#include "EventHandler.h"
class EventHandler;
class Player;

class GameObject {
protected:
	/** The shape that represents game objects. */
	sf::RectangleShape* shape;

	/** The pointer to the object that handles events for this game object*/
	EventHandler* handler;
	
public:
	int guid;
	int viewOffset;
	/**
	* Constructs a GameObject.
	*/
	GameObject();
	
	/**
	* Gets this GameObject's shape.
	* @return this GameObject's shape.
	*/
	sf::RectangleShape* getShape();


	/**
	* Gets this GameObject's event handler.
	* @return a pointer to the game object's event handler.
	*/
	EventHandler* getHandler();


};

/**
* Class that represents a platform, static or moving.
* Inherits from GameObject -> a Platform is a type of GameObject.
*/
class Platform : public GameObject {
protected:
	/** Tells whether or not this platform should be drawn. Moot at this point. */
	bool isVisible;

	/** The x velocity of the platform. */
	double xVelocity;

	/** The y velocity of the platform. */
	double yVelocity;

	/** The amount of time it takes for a platform to complete one full cycle (back and forth). */
	double period;

	/** The total amount of time passed, used to compare against the period. */
	double timePassed;

	/** The string that represents this GameObject's texture. */
	std::string texture;

public:
	/**
	* Constructs a platform object.
	* @param shape a pointer to the shape that this platform will resemble.
	* @param isVisible tells whether or not to draw this platform (moot)
	* @param xVelocity the x velocity to give this platform.
	* @param yVelocity the y velocity to give this platform.
	* @param period the period to give this platform.
	* @param texture the texture to give this platform. Not working atm.
	*/
	Platform(sf::RectangleShape* shape, bool isVisible, double xVelocity, double yVelocity, double period, std::string texture);

	/**
	* Gets isVisible.
	* @return true if visible, false otherwise.
	*/
	bool getIsVisible();

	/**
	* Sets isVisible.
	* @param isVisible the thing to set isVisible to.
	*/
	void setIsVisible(bool isVisible);

	/**
	* A thread calls this function every time to handle each platform's movement.
	* @param deltaT the amount of time that has passed since the platform was able to move last.
	*/
	void move(double deltaT);

	/**
	* Gets the x velocity for this platform.
	* @return the x velocity for this platform.
	*/
	double getXVelocity();

	/**
	* Gets the y velocity for this platform.
	* @return the y velocity for this platform.
	*/
	double getYVelocity();

	/**
	* Sets the x velocity for this platform.
	* @param xVel the x velocity to give this platform.
	*/
	void setXVelocity(double xVel);

	/**
	* Sets the y velocity for this platform.
	* @param yVel the y velocity to give this platform.
	*/
	void setYVelocity(double yVel);

	/**
	* Creates an sf::texture and gives it to this platform.
	* @param tex the string that represents the picture in the file system.
	*/
	void setTexture(std::string tex);

	/**
	* Puts this platform in an easy to iterate string format.
	* @return the string format of this platform.
	*/
	std::string to_string();

};

class Spawn : public GameObject {
public:
	/**
	* Constructs a spawn.
	* @param xPos the x position to give the spawn.
	* @param yPos the y position to give the spawn.
	*/
	Spawn(float xPos, float yPos);

	/**
	* Causes a player to teleport to this spawnpoint.
	* @param player a pointer to a player.
	*/
	void respawn(Player* player);

	/**
	* Returns an easy to iterate string of this spawn.
	* @return an easy to iterate string of this spawn.
	*/
	std::string to_string();
};

/**
* There will be two types of boundaries:
* Ones that kill and ones that don't.
* The client will repeatedly check if they're colliding with any boundary.
* If they're colliding with a death boundary the player will respawn.
*/
class Boundary : public GameObject {
private:
	int spawnIndex;
public:
	/**
	* Constructs a boundary w/ a given size and position.
	* @param width the width to give this boundary.
	* @param height the height to give this boundary.
	* @param xPos the x position to give this boundary.
	* @param yPos the y position to give this boundary.
	* @param spawnIndex the index that the player will respawn at if the player touches this and it's a kill boundary.
	*/
	Boundary(float width, float height, float xPos, float yPos, int spawnIndex);

	/**
	* Gets the spawn index for this boundary.
	*/
	int getSpawnIndex();

	/**
	* Returns an easy to iterate string of this boundary.
	* @return an easy to iterate string of this boundary.
	*/
	std::string to_string();


};
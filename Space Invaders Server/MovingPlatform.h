#include <SFML/Graphics.hpp>
#include <mutex>

class MovingPlatform : public sf::RectangleShape {
private:
	bool canCollide;
	double velocity;
	bool isMovingRight;

public:
	/**
	* Constructs a moving platform with the given directions.
	* @param vector the dimensions of the rectangle shape.
	*/
	MovingPlatform(sf::Vector2f vector);

	/**
	* Tells whether or not it will collide (Not sure when to use)
	*/
	bool doesCollide();

	/**
	* Gets the velocity of the platform
	* @return the velocity of the platform
	*/
	double getVelocity();

	/**
	* Sets whether or not it will collide (not sure when to use)
	* @param canCollide the param
	*/
	void setCollides(bool canCollide);

	/**
	* Sets the velocity of the of the platform
	* @param velocity the velocity
	*/
	void setVelocity(double velocity);

	/**
	* Sets whether or not the platform is moving left or right.
	* @param isMovingRight whether or not the platform is moving right.
	*/
	void setIsMovingRight(bool isMovingRight);

	/**
	* Gets the status of the nature of the moving platform's motion.
	* @return true if the platform is moving right, false otherwise.
	*/
	bool getIsMovingRight();

	/**
	* Gets the string for this object.
	* @return the string representation of this object.
	*		  Order is:
	*		  x position: a float representing the X position of the platform
	*		  y position: a float representing the Y position of the platform
	*		  width: a float representing the width
	*		  height: a float representing the height
	*		  isMovingRight: 0 or 1
	*		  velocity: float representation of velocity.
	*		  r: red component of platform
	*		  g: green component of platform
	*		  b: blue component of platform
	*/
	std::string to_string();
};

#include <SFML/Graphics.hpp>

class StaticPlatform : public sf::RectangleShape {
private:
	bool canCollide;

public:
	/**
	* Constructs a static platform, inherits from RectangleShape.
	* @param vector the dimensions of the rectangle.
	*/
	StaticPlatform(sf::Vector2f vector);

	/**
	* Checks the collision (don't know)
	* @return whether or not it collides.
	*/
	bool doesCollide();

	/**
	* Sets the collision.
	* @param canCollide sets the collide.
	*/
	void setCollides(bool canCollide);

	/**
	* Gets le string for this static platform.
	*         width: a float representing the width of the platform.
	*         height: a float representing the height of the platform.
	*		  x position: a float representing the X position of the platform
	*		  y position: a float representing the Y position of the platform
	*		  r: red component of platform
	*		  g: green component of platform
	*		  b: blue component of platform
	*/
	std::string to_string();
};


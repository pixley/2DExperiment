#ifndef _CAMERA_H
#define _CAMERA_H

#include <SFML\Graphics.hpp>
#include "level.h"

//The camera through which the level is viewed.
class Camera
{
private:
	//Absolute position of camera: # of pixels from origin of level map
	sf::Vector2f position;

	//Target position camera is moving towards
	sf::Vector2f target;

	//Size of camera
	sf::Vector2i size;

	//Speed of camera, a value between 0 and 1
	float speed;

	//current level
	Level* currentLevel;

	//void MoveChecker(int* x, int* y);

public:
	Camera(int w, int h, float speed, Level* level);
	~Camera();

	//Moves camera immediately to coordinates (x,y)
	void Move(int x, int y);
	void MoveCenter(int x, int y);

	//Sets camera target
	void GoTo(int x, int y);
	void GoToCenter(int x, int y);

	//Changes camera speed
	void SetSpeed (float v);

	//Updates camera position
	void Update();

	sf::Vector2i GetPosition() { return sf::Vector2i((int)position.x, (int)position.y); }

	sf::Vector2f GetRealPosition() { return position; };

	//Helper function for retreiving camera's offset from nearest tile
	sf::Vector2i TileOffset(int tileSize);

	//Helper function for retreiving a rectangle defining which tiles are visible through camera
	sf::IntRect TileBounds(int tileSize);
};

#endif

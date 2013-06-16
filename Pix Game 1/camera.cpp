#include <SFML\Graphics.hpp>
#include <math.h>
#include "camera.h"
#include "level.h"
#include "texturemanager.h"

Camera::Camera(int w, int h, float speed, Level* level)
{
	size.x = w;
	size.y = h;

	if(speed < 0.0)
		speed = 0.0;
	if(speed > 1.0)
		speed = 1.0;

	this->speed = speed;
	this->currentLevel = level;
}

Camera::~Camera()
{

}

//void Camera::MoveChecker(int* x, int* y)
//{
//	if(*x < size.x / 2)
//		*x = size.x / 2;
//	if(*x > (currentLevel->GetWidth() * TILESIZE) - (size.x / 2))
//		*x = (currentLevel->GetWidth() * TILESIZE) - (size.x / 2);
//	if(*y < size.y / 2)
//		*y = size.y / 2;
//	if(*y > (currentLevel->GetHeight() * TILESIZE) - (size.y / 2))
//		*y = (currentLevel->GetHeight() * TILESIZE) - (size.y / 2);
//}

void Camera::Move(int x, int y)
{
	//MoveChecker(&x, &y);

	position.x = (float)x;
	position.y = (float)y;

	target.x = (float)x;
	target.y = (float)y;
}

void Camera::MoveCenter(int x, int y)
{
	//MoveChecker(&x, &y);

	x = x - (size.x / 2);
	y = y - (size.y / 2);

	position.x = (float)x;
	position.y = (float)y;

	target.x = (float)x;
	target.y = (float)y;
}

void Camera::GoTo(int x, int y)
{
	//MoveChecker(&x, &y);

	target.x = (float)x;
	target.y = (float)y;
}

void Camera::GoToCenter(int x, int y)
{
	//MoveChecker(&x, &y);

	x = x - (size.x / 2);
	y = y - (size.y / 2);

	target.x = (float)x;
	target.y = (float)y;
}

void Camera::SetSpeed(float v)
{
	if(v < 0.0)
		v = 0.0;
	if(v > 1.0)
		v = 1.0;
	speed = v;
}

//This allows the camera to scroll towards the target over time if desired
void Camera::Update()
{
	//X distance to target, Y distance to target, and D Euclidian distance to target
	float x, y, d;

	//Velocity magnitudes
	float vx, vy, v;

	//Find x and y
	x = (float)(target.x - position.x);
	y = (float)(target.y - position.y);

	//Check if within 1 pixel of target for instant snap, else continue
	if ((x*x + y*y) <= 1)
	{
		position.x = target.x;
		position.y = target.y;
	}
	else if((position.x != target.x) && (position.y != target.y))
	{
		d = sqrt(((x * x) + (y * y)));

		/* Set velocity to move d/60 to the target.  60 is arbitrary.  We allow user to set camera
			speed with SPEED member. */
		v = d * speed;

		//V must stay above 1 pixel per update, else TARGET may not be reached.  V is nonnegative.
		if(v < 1.0f)
			v = 1.0f;

		//Get VX and VY
		vx = x * (v / d);
		vy = y * (v / d);

		//Update position
		position.x += vx;
		position.y += vy;
	}
}

sf::Vector2i Camera::GetTileOffset(int tileSize)
{
	return sf::Vector2i((int)(position.x) % tileSize, (int)(position.y) % tileSize);
}

sf::IntRect Camera::GetTileBounds(int tileSize)
{
	int x = (int)position.x / tileSize;
	int y = (int)position.y / tileSize;

	//The +2 exists to have all values start at 1 instead of 0, and to make sure division is nonzero.
	int w = size.x / tileSize + 1;
	int h = size.y / tileSize + 1;

	//Plus 1 more if camera is offset from tile
	if(x % tileSize != 0)
		w++;
	if(y % tileSize != 0)
		h++;

	return sf::IntRect(x, y, w, h);
}

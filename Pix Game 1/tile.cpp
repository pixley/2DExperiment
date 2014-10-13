#include "tile.h"
#include <SFML\Graphics.hpp>

Tile::Tile(sf::Texture& texture, bool wall, int x, int y)
{
	baseSprite.setTexture(texture, true);
	walkable = !wall;
	occupant = NULL;
	subOccupant = NULL;
	domOccupant = NULL;
	loc.x = x;
	loc.y = y;
	travelled = 0x7fffffff;
	priority = 0x7fffffff;
	closed = false;
	path = NULL;
	magic = 0x12345678;
}

Tile::~Tile()
{
	if(path)
		delete[] path;
}

void Tile::Draw(int x, int y, sf::RenderWindow* rw)
{
	baseSprite.setPosition(x, y);
	rw->draw(baseSprite);
}

void Tile::DrawOccupants(float x, float y, sf::RenderWindow* window) {
	if (occupant)
	{
		occupant->Draw(window, x, y);
	}
	if (domOccupant)
	{
		domOccupant->Draw(window, x, y);
	}
}

bool Tile::IsWalkable()
{
	return walkable && (domOccupant == NULL);
}

bool Tile::isTile()
{
	return magic == 0x12345678;
}

Entity* Tile::GetOccupant()
{
	return occupant;
}

void Tile::SetOccupant(Entity* entity)
{
	occupant = entity;
}

Actor* Tile::GetDomOccupant()
{
	return domOccupant;
}

void Tile::SetDomOccupant(Actor* actor)
{
	domOccupant = actor;
}

Actor* Tile::GetSubOccupant()
{
	return subOccupant;
}

void Tile::SetSubOccupant(Actor* actor)
{
	subOccupant = actor;
}

sf::Vector2i Tile::GetLoc()
{
	return loc;
}

void Tile::SetTexture(sf::Texture& texture)
{
	baseSprite.setTexture(texture, true);
}

int Tile::GetDist()
{
	return travelled;
}

void Tile::UpdateDist(int x)
{
	if (x < travelled)
		travelled = x;
}

int Tile::GetPriority()
{
	return priority;
}

void Tile::UpdatePriority(int x, int y)
{
	priority = travelled + Estimate(x, y);
}

int Tile::Estimate(int x, int y)
{
	return (int)sqrt((float)((x - loc.x) * (x - loc.x) + (y - loc.y) * (y - loc.y)));
}

void Tile::ClearNode()
{
	travelled = 0x7fffffff;
	priority = 0x7fffffff;
	closed = false;
}

bool Tile::IsReachable(int speed)
{
	return (travelled <= speed) && IsWalkable();
}

void Tile::SetClosed(bool close)
{
	closed = close;
}

bool Tile::IsClosed()
{
	return closed;
}

void Tile::SetPath(int* directions)
{
	if (path)
		delete path;

	path = directions;
}

int* Tile::GetPath()
{
	return path;
}


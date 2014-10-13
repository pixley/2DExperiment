#ifndef _TILE_H
#define _TILE_H

#include <SFML\Graphics.hpp>
#include "entity.h"
#include "actor.h"

/*
The tile is both an element of the level layout and from where entities are referenced.
Pathfinding data is stored here, and is referenced in A* through the tilenode class.
*/
class Tile
{
private:
	sf::Sprite baseSprite;
	bool walkable;
	sf::Vector2i loc;
	int magic;
	int* path;

	Entity* occupant;
	Actor* domOccupant;
	Actor* subOccupant;

	//A*-related variables
	//Distance travelled from starting point
	int travelled;
	//Priority = travelled + remaining euclidian distance; smaller value is better
	int priority;
	//Indicates whether tile has been fully explored by A*
	bool closed;

public:
	Tile(sf::Texture& texture, bool wall, int x, int y);
	~Tile();

	void Draw(int x, int y, sf::RenderWindow* rw);
	void DrawOccupants(float x, float y, sf::RenderWindow* window);

	bool IsWalkable();

	bool isTile();

	Entity* GetOccupant();
	void SetOccupant(Entity* entity);

	void SetTexture(sf::Texture& texture);

	//Primary occupant and controller of a tile
	Actor* GetDomOccupant();
	void SetDomOccupant(Actor* actor);

	//Secondary occupant of a tile; usually subdued by dominant occupant, such as grappling
	Actor* GetSubOccupant();
	void SetSubOccupant(Actor* actor);

	//Get location of tile
	sf::Vector2i GetLoc();

	//A*-related functions
	int GetDist();
	void UpdateDist(int x);
	int GetPriority();
	void UpdatePriority(int x, int y);
	int Estimate(int x, int y);
	void ClearNode();
	bool IsReachable(int speed);
	void SetClosed(bool close);
	bool IsClosed();
	void SetPath(int* directions);
	int* GetPath();
};


#endif
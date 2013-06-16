#ifndef _TILE_H
#define _TILE_H

#include <SFML\Graphics.hpp>
#include "entity.h"

class Tile
{
private:
	sf::Sprite baseSprite;
	bool walkable;
	int magic;

	Entity* occupant;
	//Actor* domOccupant;
	//Actor* subOccupant;

public:
	Tile(sf::Texture& texture, bool walk);
	~Tile();

	void Draw(int x, int y, sf::RenderWindow* rw);

	bool isWalkable();

	bool isTile();

	Entity* GetOccupant();
	void SetOccupant(Entity* entity);

	void SetTexture(sf::Texture& texture);

	//Primary occupant and controller of a tile
	//Actor* GetDomOccupant();
	//void SetDomOccupant(Actor* actor);

	//Secondary occupant of a tile; usually subdued by dominant occupant, such as grappling
	//Actor* GetSubOccupant();
	//void SetSubOccupant(Actor* actor);
};

#endif
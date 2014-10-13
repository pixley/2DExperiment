#ifndef _TILENODE_H
#define _TILENODE_H

#include "tile.h"

/*
The tilenode is a layer of abstraction over the tile for the purposes of pathfinding
using A*.  It allows for placing in the priority queue and for syntax-adherent
comparisons between tiles' priorities within.
*/
class TileNode
{
private:
	Tile* tile;
public:
	TileNode(Tile* t);
	~TileNode();

	Tile* operator->();
	Tile* GetTile();
};

bool operator<(TileNode a, TileNode b);

#endif
#include "tilenode.h"

TileNode::TileNode(Tile* t)
{
	tile = t;
}

TileNode::~TileNode()
{

}

Tile* TileNode::operator->()
{
	return tile;
}

Tile* TileNode::GetTile()
{
	return tile;
}

bool operator<(TileNode a, TileNode b)
{
	return a.GetTile()->GetPriority() > b.GetTile()->GetPriority();
}

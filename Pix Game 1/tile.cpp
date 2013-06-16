#include "tile.h"
#include <SFML\Graphics.hpp>

Tile::Tile(sf::Texture& texture, bool walk)
{
	baseSprite.setTexture(texture, true);
	walkable = walk;
	occupant = NULL;
	magic = 0x12345678;
}

Tile::~Tile()
{

}

void Tile::Draw(int x, int y, sf::RenderWindow* rw)
{
	baseSprite.setPosition(x, y);
	rw->draw(baseSprite);
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

void Tile::SetTexture(sf::Texture& texture)
{
	baseSprite.setTexture(texture, true);
}

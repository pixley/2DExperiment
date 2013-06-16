#ifndef _LEVEL_H
#define _LEVEL_H

#include <vector>
#include <string>
#include "tile.h"
#include "texturemanager.h"
#include "entity.h"
#include "rapidxml\rapidxml.hpp"
#include <SFML\Graphics.hpp>
using namespace std;

class Level
{
private:
	//A 2D array of Tile pointers
	std::vector<std::vector<Tile*>> map;

	//Entities in the level
	std::vector<Entity*> entities;
	int entCount;

	//Width and height of level in tiles
	int w;
	int h;
	
	//Texture Manager
	TextureManager textureManager;

	//Currently selected tile
	Tile* selectedTile;
	sf::Sprite selectorSprite;

	//Currently hovered tile
	Tile* hoverTile;
	sf::Sprite hoverSprite;

	//Sprite to draw Action Mode grid
	sf::Sprite gridSprite;

	void SetDimensions(int w, int h);

public:
	Level(int w, int h);
	~Level();

	//Tile creation and retreival
	void AddTile(unsigned int x, unsigned int y, Tile* tile);
	Tile* GetTile(unsigned int x, unsigned int y);

	//Tile selection
	void SelectTile(Tile* tile);
	bool IsSelectedTile(Tile* tile);
	void ClearSelect();
	void DrawSelectBorder(int x, int y, sf::RenderWindow* window);

	//Tile hovering
	void HoverTile(Tile* tile);
	bool IsHoveredTile(Tile* tile);
	void ClearHover();
	void DrawHoverBorder(int x, int y, sf::RenderWindow* window);

	//Tile grid
	void DrawGridSprite(int x, int y, sf::RenderWindow* window);

	//Add entity to level
	void AddEntity(rapidxml::xml_node<>* entityXml);
	//Get entity by location
	//Entity* GetEntity(int x, int y);
	//Get entity by index
	Entity* GetEntity(int i);

	void LoadLevel(std::string filename);
	void UnloadLevel();

	int GetWidth();
	int GetHeight();
};

#endif

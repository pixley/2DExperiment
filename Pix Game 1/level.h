#ifndef _LEVEL_H
#define _LEVEL_H

#include <vector>
#include <string>
#include "tile.h"
#include "texturemanager.h"
#include "entity.h"
#include "actor.h"
#include "rapidxml\rapidxml.hpp"
#include <SFML\Graphics.hpp>

//Direction helpers
static int dx[4] = {0, 0, 1, -1};
static int dy[4] = {1, -1, 0, 0};

//The game level.  Any reference to the tile map or entities goes through here.  Also handles A* pathfinding.
class Level
{
private:
	//A 2D array of Tile pointers
	std::vector<std::vector<Tile*>> map;

	//Entities in the level
	std::vector<Entity*> entities;
	int entCount;

	//Actors in the level
	std::vector<Actor*> actors;
	int actCount;

	//Width and height of level in tiles
	int w;
	int h;
	
	//Texture Manager
	TextureManager textureManager;

	//Currently selected tile
	Tile* selectedTile;
	sf::Sprite selectorSprite;

	//Currently selected actor
	Actor* selectedActor;
	//Sprite to indicate valid moves for selected actor
	sf::Sprite moveSprite;
	//Sprite to indicate invalid moves for selected actor
	sf::Sprite noMoveSprite;

	//Currently hovered tile
	Tile* hoverTile;
	sf::Sprite hoverSprite;

	//Sprite to draw Action Mode grid
	sf::Sprite gridSprite;

	void SetDimensions(int w, int h);

	//Helper function for Update()
	void UpdatePos(Actor* actor);

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
	Tile* GetSelectedTile();

	//Actor selection
	Actor* GetSelectedActor();
	void SetSelectedActor(Actor* actor);
	void ClearSelectedActor();

	//Tile hovering
	void HoverTile(Tile* tile);
	bool IsHoveredTile(Tile* tile);
	void ClearHover();
	void DrawHoverBorder(int x, int y, sf::RenderWindow* window);

	//Tile grid
	void DrawGridSprite(int x, int y, sf::RenderWindow* window);

	//Show valid movement tiles
	void DrawMoveSprites(int x, int y, int speed, Tile* tile, sf::RenderWindow* window);
	void ClearNodes();
	void OpenNodes();

	//Add entity to level
	void AddEntity(rapidxml::xml_node<>* root);

	//Get entity by index
	Entity* GetEntity(int i);

	//Get actor by index
	Actor* GetActor(int i);

	//Add actor to level
	void AddActor(rapidxml::xml_node<>* root);

	void LoadLevel(std::string filename);
	void UnloadLevel();

	int GetWidth();
	int GetHeight();

	//Update positions of actors in level
	void Update();

	bool ReachTile(Actor* actor, Tile* tile);

	void GetPaths(int x, int y, int speed);

	int* Pathfind(int x0, int y0, int x, int y, int speed);
};

#endif

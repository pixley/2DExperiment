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
using namespace sf;

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
	Sprite selectorSprite;

	//Currently targeted tile
	Tile* targetedTile;
	Sprite targetSprite;

	//Sprite to indicate valid moves for selected actor
	Sprite moveSprite;
	//Sprite to indicate invalid moves for selected actor
	Sprite noMoveSprite;

	//Currently hovered tile
	Tile* hoverTile;
	Sprite hoverSprite;

	//Sprite to draw Action Mode grid
	Sprite gridSprite;

	void SetDimensions(int w, int h);

	//Helper function for Update()
	void UpdatePos(Actor* actor);

	//Helper functions for Draw()
	void DrawSelectBorder(int x, int y, sf::RenderWindow* window);
	void DrawTargetBorder(int x, int y, sf::RenderWindow* window);
	void DrawHoverBorder(int x, int y, sf::RenderWindow* window);
	void DrawGridSprite(int x, int y, sf::RenderWindow* window);
	void DrawMoveSprites(int x, int y, int speed, Tile* tile, sf::RenderWindow* window);

public:
	Level(std::string filename);
	~Level();

	//Drawing the level
	void Draw(bool actionMode, Vector2i camOffset, IntRect bounds, Vector2f camPos, sf::RenderWindow* window);

	//Tile creation and retreival
	void AddTile(unsigned int x, unsigned int y, Tile* tile);

	//Tile selection
	void SelectTile(unsigned int x, unsigned int y);
	void ClearSelect();

	//Tile targeting
	void TargetTile(unsigned int x, unsigned int y);

	//Tile hovering
	void HoverTile(unsigned int x, unsigned int y);

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

	//Update positions of actors in level
	void Update();

	//Switches to and from Action Mode
	bool SwitchMode(bool actionMode);

	bool ReachTile(Actor* actor, Tile* tile);

	void GetPaths(int x, int y, int speed);

	int* Pathfind(int x0, int y0, int x, int y, int speed);
};

#endif

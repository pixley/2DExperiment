#ifndef _ENTITY_H
#define _ENTITY_H

#include <SFML\graphics.hpp>
#include <string>
#include <vector>
#include "texturemanager.h"
#include "rapidxml\rapidxml.hpp"

enum interact
{
	LOOK,
	TALK,
	ATTACK,
	TAKE,
	STEAL,
	OVERTURN,
	USE,
	DISABLE,
	HACK,
	CLIMB,
	JUMP,
	LIFT,
	UNLOCK,
	LOCK,
	BOARD,
	REPAIR,
	HEAL,
	EXTINGUISH,
	READ,
	MOVE,
	PILOT,
	LOOT,
	INSPECT
};

enum facing
{
	SOUTH,
	NORTH,
	EAST,
	WEST
};

class Entity
{
private:
	//real position
	sf::Vector2f position;
	//tile location
	sf::Vector2i tilePosition;
	//sprite
	sf::Sprite baseSprite;

	//name of entity
	std::string name;

	//pointer to texture manager
	TextureManager textureManager;

	//list of valid actions that can be made on entity
	bool actions[INSPECT];

	//direction entity is facing
	facing dir;

	//helper functions for loader
	void loadSpriteSheets(rapidxml::xml_node<>* spritesheet);
	void loadValidActions(rapidxml::xml_node<>* validAct);

public:
	Entity();
	~Entity();

	void LoadEntity(rapidxml::xml_node<>* entityXml);

	//checks if ACTION can be made on entity
	bool Interact(interact action);
	
	void changeSpriteSheet(int sheetNum);

	void Draw(int x, int y, sf::RenderWindow* rw);

	sf::Vector2i GetPosition();

	sf::Vector2f GetRealPosition();
};

#endif

#include "entity.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "rapidxml\rapidxml.hpp"
#include "texturemanager.h"
#include "camera.h"

Entity::Entity()
{
	position.x = 0;
	position.y = 0;

	tilePosition.x = 0;
	tilePosition.y = 0;

	name = "non-loaded entity";

	dir = SOUTH;
}

Entity::~Entity()
{

}

void Entity::LoadEntity(rapidxml::xml_node<>* root)
{
	if(!root)
		throw "Entity XML file not found";

	//set name
	name = root->first_attribute("name", 0, true)->value();

	//Load all spritesheets and set first sheet
	rapidxml::xml_node<>* spritesheet = root->first_node("spritesheet", 0, true);
	loadSpriteSheets(spritesheet);

	//load entity's valid actions
	rapidxml::xml_node<>* validAct = root->first_node("validAct", 0, true);
	loadValidActions(validAct);

	//set facing
	rapidxml::xml_node<>* describer = root->first_node("describer", 0, true);
	int direction = atoi(describer->first_attribute("dir", 0, true)->value());
	switch (direction) {
	default:
	case 0:
		dir = SOUTH;
		break;
	case 1:
		dir = NORTH;
		break;
	case 2:
		dir = EAST;
		break;
	case 3:
		dir = WEST;
		break;
	}

	//set occupied tile
	tilePosition.x=atoi(describer->first_attribute("x", 0, true)->value());
	tilePosition.y=atoi(describer->first_attribute("y", 0, true)->value());

	//set sprite to only show one frame of spritesheet
	baseSprite.setTextureRect(sf::IntRect(TILESIZE * dir, 0, TILESIZE, TILESIZE * 2));

	//anchor sprite to top-left corner of bottom-center tile
	baseSprite.setOrigin(0.0f, 40.0f);

	//set real position
	position.x = (float)(tilePosition.x * TILESIZE);
	position.y = (float)(tilePosition.y * TILESIZE);
}

void Entity::loadSpriteSheets(rapidxml::xml_node<>* spritesheet)
{
	if(!spritesheet)
		throw "No reference to spritesheet";
	while(spritesheet)
	{
		int w = atoi(spritesheet->first_attribute("w", 0, true)->value());
		int h = atoi(spritesheet->first_attribute("h", 0, true)->value());
		//load spritesheet
		textureManager.LoadSpriteSheet(spritesheet, w, h);

		spritesheet = spritesheet->next_sibling("spritesheet", 0, true);
	}

	baseSprite.setTexture(textureManager.GetTexture(0), false);
}

void Entity::loadValidActions(rapidxml::xml_node<>* validAct)
{
	if(!validAct)
		throw "No reference to valid actions";
	actions[LOOK] = (validAct->first_attribute("look", 0, true)->value() == "true") ? true : false;
	actions[TALK] = (validAct->first_attribute("talk", 0, true)->value() == "true") ? true : false;
	actions[ATTACK] = (validAct->first_attribute("attack", 0, true)->value() == "true") ? true : false;
	actions[TAKE] = (validAct->first_attribute("take", 0, true)->value() == "true") ? true : false;
	actions[STEAL] = (validAct->first_attribute("steal", 0, true)->value() == "true") ? true : false;
	actions[OVERTURN] = (validAct->first_attribute("overturn", 0, true)->value() == "true") ? true : false;
	actions[USE] = (validAct->first_attribute("use", 0, true)->value() == "true") ? true : false;
	actions[DISABLE] = (validAct->first_attribute("disable", 0, true)->value() == "true") ? true : false;
	actions[HACK] = (validAct->first_attribute("hack", 0, true)->value() == "true") ? true : false;
	actions[CLIMB] = (validAct->first_attribute("climb", 0, true)->value() == "true") ? true : false;
	actions[JUMP] = (validAct->first_attribute("jump", 0, true)->value() == "true") ? true : false;
	actions[LIFT] = (validAct->first_attribute("lift", 0, true)->value() == "true") ? true : false;
	actions[UNLOCK] = (validAct->first_attribute("unlock", 0, true)->value() == "true") ? true : false;
	actions[LOCK] = (validAct->first_attribute("lock", 0, true)->value() == "true") ? true : false;
	actions[BOARD] = (validAct->first_attribute("board", 0, true)->value() == "true") ? true : false;
	actions[REPAIR] = (validAct->first_attribute("repair", 0, true)->value() == "true") ? true : false;
	actions[HEAL] = (validAct->first_attribute("heal", 0, true)->value() == "true") ? true : false;
	actions[FIGHT_FIRE] = (validAct->first_attribute("fight_fire", 0, true)->value() == "true") ? true : false;
	actions[READ] = (validAct->first_attribute("read", 0, true)->value() == "true") ? true : false;
	actions[MOVE] = (validAct->first_attribute("move", 0, true)->value() == "true") ? true : false;
	actions[PILOT] = (validAct->first_attribute("pilot", 0, true)->value() == "true") ? true : false;
	actions[LOOT] = (validAct->first_attribute("loot", 0, true)->value() == "true") ? true : false;
	actions[INSPECT] = (validAct->first_attribute("inspect", 0, true)->value() == "true") ? true : false;
	actions[IGNITE] = (validAct->first_attribute("ignite", 0, true)->value() == "true") ? true : false;
	actions[DEMOLISH] = (validAct->first_attribute("demolish", 0, true)->value() == "true") ? true : false;
	actions[DISGUISE] = (validAct->first_attribute("disguise", 0, true)->value() == "true") ? true : false;
	actions[CONCEAL] = (validAct->first_attribute("conceal", 0, true)->value() == "true") ? true : false;
	actions[OBSERVE] = (validAct->first_attribute("observe", 0, true)->value() == "true") ? true : false;
}

bool Entity::Interact(interact action)
{
	return actions[action];
}

void Entity::Draw(sf::RenderWindow* rw, float camX, float camY)
{
	baseSprite.setPosition((int)(position.x - camX), (int)(position.y - camY));
	rw->draw(baseSprite);
}

sf::Vector2i Entity::GetPosition()
{
	return tilePosition;
}

sf::Vector2f Entity::GetRealPosition()
{
	return position;
}

void Entity::SetDir(int face)
{
	switch (face){
	case 0:
		dir = SOUTH;
		break;
	case 1:
		dir = NORTH;
		break;
	case 2:
		dir = EAST;
		break;
	case 3:
		dir = WEST;
		break;
	default:
		break;
	}

	baseSprite.setTextureRect(sf::IntRect(TILESIZE * dir, 0, TILESIZE, TILESIZE * 2));
}

int Entity::GetDir()
{
	return dir;
}

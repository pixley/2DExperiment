#ifndef _ENTITY_H
#define _ENTITY_H

#include <SFML\graphics.hpp>
#include <string>
#include <vector>
#include "texturemanager.h"
#include "rapidxml\rapidxml.hpp"

static int MAX_FRAME = 8;

//The types of actions that may be taken by an actor or performed on an entity.
enum interact
{
	LOOK,
	LISTEN,
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
	FIGHT_FIRE,
	READ,
	WRITE,
	MOVE,
	SLIDE,
	PILOT,
	LOOT,
	INSPECT,
	IGNITE,
	DEMOLISH,
	DISGUISE,
	CONCEAL,
	OBSERVE,
	GET_ITEM,
	STOW,
	DRAW,
	HIDE,
	SNEAK,
	DASH,
	WALK,
	JOG,
	CROUCH,
	PRONE,
	STAND,
	TRACK,
	SWIM,
	CRAFT,
	END_OF_ACTS
};

enum facing
{
	SOUTH,
	NORTH,
	EAST,
	WEST
};

enum animation
{
	IDLE,
	WALKING,
	RUNNING,
	JUMPING,
	RUNNINGJUMP,
	FALLING,
	LANDING,
	FALLOVER,
	SITTING,
	DRIVING,
	TALKING,
	SITTALKING,
	YELLING,
	SITYELLING,
	DEATH
};

/*
Entities are the pieces on the board represented by the tiles.  Represented by sprites,
which are handled by each entity's own texture manager.
*/
class Entity
{
protected:
	//real position
	sf::Vector2f position;
	//tile location
	sf::Vector2i tilePosition;
	//sprite
	sf::Sprite baseSprite;

	//name of entity
	std::string name;

	//index of entity's spritesheet in the Texture Manager
	unsigned int texIdx;

	//list of valid actions that can be made on entity
	bool actions[END_OF_ACTS];

	//direction entity is facing
	facing dir;

	//Entity's current animation
	animation anim;

	//Frame of entity's current animation (0-8)
	int frame;

	//helper functions for loader
	void loadSpriteSheets(rapidxml::xml_node<>* spritesheet, TextureManager& tm);
	void loadValidActions(rapidxml::xml_node<>* validAct);

public:
	Entity(rapidxml::xml_node<>* root, TextureManager& tm);
	~Entity();

	//checks if ACTION can be made on entity
	bool Interact(interact action);
	
	void changeSpriteSheet(int sheetNum);

	void Draw(sf::RenderWindow* rw, float camX, float camY);

	sf::Vector2i GetPosition();

	sf::Vector2f GetRealPosition();

	void SetDir(int face);
	int GetDir();

	void SetAnim(int ani);
	int GetAnim();

	void AdvanceFrame();
};

#endif

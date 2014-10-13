#ifndef _ACTOR_H
#define _ACTOR_H

#include "entity.h"
#include "rapidxml\rapidxml.hpp"
#include <string>
#include <SFML\graphics.hpp>

//Used to index the Actor::skills array
enum skills
{
	HEAVY_LIFTING,
	HEAVY_WEAPONS,
	BOWS,
	HEAVY_MELEE,
	SMALL_ARMS,
	INCENDIARY,
	MEDIUM_ARMS,
	THROWN,
	RECOIL_CONTROL,
	SPRINT,
	PROWL,
	SKIING,
	QUICK_DRAW,
	SCALING_CLIMBING,
	REACTION,
	LIGHT_MELEE,
	COMBAT_SLIDE,
	FREE_RUNNING,
	BALANCE,
	BASE_JUMPING_PARACHUTING,
	NULLO,
	CONCEALMENT,
	HAND_TO_HAND,
	PAIN_TOLERANCE,
	RUNNING_ENDURANCE,
	SWIMMING_ENDURANCE,
	HOLDING_BREATH,
	RESIST_INFECTION,
	FASTING,
	FORCED_INSOMNIA,
	OBSERVER,
	SEARCH,
	FISHING,
	NAVIGATION,
	TRACKING,
	LOCKPICKING_PICKPOCKETING,
	DISGUISES,
	CAMOUFLAGE,
	ARC_WEAPONS,
	PRESERVE_FOOD,
	MOTORCYCLE,
	CAR,
	CARGO_VEHICLE,
	HORSEMANSHIP,
	STREETWISE,
	FIRST_AID,
	PURIFY_WATER,
	EXTINGUISH,
	PUBLIC_SPEAKING,
	INSTRUCTION,
	LEADERSHIP,
	PERSUASION,
	MIMIC_VOICE,
	BARTER,
	RALLY,
	THREATEN,
	BIOLOGY,
	CHEMISTRY,
	PROGRAMMING,
	GEOLOGY,
	MEDICINE,
	BASIC_MECHANICS,
	EXPERT_MECHANICS,
	METEOROLOGY,
	METALLURGY,
	BOTANY,
	MATHEMATICS,
	TANK_APC,
	SMALL_BOAT,
	LARGE_BOAT,
	SHUTTLE,
	STARSHIP,
	LITERACY,
	WRITING,
	EXPLOSIVES,
	PHYSICS,
	ELECTRICAL_ENGINEERING,
	CARPENTRY,
	END_OF_SKILLS
};

//Used to index the Actor::stats array
enum stats
{
	STRENGTH,
	STAMINA,
	CHARISMA,
	AGILITY,
	INTELLIGENCE,
	EDUCATION,
	AURA,
	END_OF_STATS
};

//Used to index the Actor::hp array
enum parts
{
	LARM,
	RARM,
	LLEG,
	RLEG,
	TORSO,
	HEAD,
	END_OF_PARTS
};

class Actor : public Entity
{
protected:
	//Is this actor alive?
	bool alive;

	//Is this actor completely within a tile?
	bool anchored;

	//Stores base thresholds for skills, on range of -1 to 100, -1 being a skill inaccessable to the actor
	int skills[END_OF_SKILLS];

	//Stores actor's stats
	int stats[END_OF_STATS];

	//hit points for each body part, first column for current HP, second for total HP
	int hp[END_OF_PARTS][2];

	//Helper functions for constructor
	void LoadSkills(rapidxml::xml_node<>* skillNode);
	void LoadStats(rapidxml::xml_node<>* statNode);
	void LoadHP(rapidxml::xml_node<>* hpNode);

	//Current destination
	sf::Vector2i dest;

	//Movement speed
	int speed;

public:
	Actor(rapidxml::xml_node<>* root, TextureManager& tm);
	~Actor();

	void SetDest(int x, int y);
	sf::Vector2i GetDest();

	bool IsAnchored();
	void SetAnchor(bool anchor);

	void Move(int x, int y);
	void SetPosition(int x, int y);
	void SetRealPosition(float x, float y);

	int GetSpeed();

	//Counter for how far the actor is along its path to its destination.
	//Used to index the destination's path array.
	int pathIdx;
};

#endif
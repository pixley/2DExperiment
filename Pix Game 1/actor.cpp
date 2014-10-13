#include "actor.h"
#include "entity.h"
#include "rapidxml\rapidxml.hpp"
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

Actor::Actor(rapidxml::xml_node<>* root, TextureManager& tm) : Entity(root, tm)
{
	anchored = true;
	speed = 8;
	pathIdx = 0;

	//Load skills
	rapidxml::xml_node<>* skillNode = root->first_node("skills", 0, true);
	LoadSkills(skillNode);

	//Load stats
	rapidxml::xml_node<>* statNode = root->first_node("stats", 0, true);
	LoadStats(statNode);

	//Load HP
	rapidxml::xml_node<>* hpNode = root->first_node("hp", 0, true);
	LoadHP(hpNode);

	dest.x = tilePosition.x;
	dest.y = tilePosition.y;
}

Actor::~Actor()
{

}

void Actor::LoadSkills(rapidxml::xml_node<>* skillNode)
{
	if(!skillNode)
		throw "Skills not referenced";

	skills[HEAVY_LIFTING] = atoi(skillNode->first_attribute("heavy_lifting", 0, true)->value());
	skills[HEAVY_WEAPONS] = atoi(skillNode->first_attribute("heavy_weapons", 0, true)->value());
	skills[BOWS] = atoi(skillNode->first_attribute("bows", 0, true)->value());
	skills[HEAVY_MELEE] = atoi(skillNode->first_attribute("heavy_melee", 0, true)->value());
	skills[SMALL_ARMS] = atoi(skillNode->first_attribute("small_arms", 0, true)->value());
	skills[INCENDIARY] = atoi(skillNode->first_attribute("incendiary", 0, true)->value());
	skills[MEDIUM_ARMS] = atoi(skillNode->first_attribute("medium_arms", 0, true)->value());
	skills[THROWN] = atoi(skillNode->first_attribute("thrown", 0, true)->value());
	skills[RECOIL_CONTROL] = atoi(skillNode->first_attribute("recoil_control", 0, true)->value());
	skills[SPRINT] = atoi(skillNode->first_attribute("sprint", 0, true)->value());
	skills[PROWL] = atoi(skillNode->first_attribute("prowl", 0, true)->value());
	skills[SKIING] = atoi(skillNode->first_attribute("skiing", 0, true)->value());
	skills[QUICK_DRAW] = atoi(skillNode->first_attribute("quick_draw", 0, true)->value());
	skills[SCALING_CLIMBING] = atoi(skillNode->first_attribute("scaling_and_climbing", 0, true)->value());
	skills[REACTION] = atoi(skillNode->first_attribute("reaction", 0, true)->value());
	skills[LIGHT_MELEE] = atoi(skillNode->first_attribute("light_melee", 0, true)->value());
	skills[COMBAT_SLIDE] = atoi(skillNode->first_attribute("slide", 0, true)->value());
	skills[FREE_RUNNING] = atoi(skillNode->first_attribute("free_running", 0, true)->value());
	skills[BALANCE] = atoi(skillNode->first_attribute("balance", 0, true)->value());
	skills[BASE_JUMPING_PARACHUTING] = atoi(skillNode->first_attribute("base_jumping_and_parachuting", 0, true)->value());
	skills[NULLO] = atoi(skillNode->first_attribute("nullo_combat", 0, true)->value());
	skills[CONCEALMENT] = atoi(skillNode->first_attribute("concealment", 0, true)->value());
	skills[HAND_TO_HAND] = atoi(skillNode->first_attribute("hand_to_hand", 0, true)->value());
	skills[PAIN_TOLERANCE] = atoi(skillNode->first_attribute("pain_tolerance", 0, true)->value());
	skills[RUNNING_ENDURANCE] = atoi(skillNode->first_attribute("running_endurance", 0, true)->value());
	skills[SWIMMING_ENDURANCE] = atoi(skillNode->first_attribute("swimming_endurance", 0, true)->value());
	skills[HOLDING_BREATH] = atoi(skillNode->first_attribute("holding_breath", 0, true)->value());
	skills[RESIST_INFECTION] = atoi(skillNode->first_attribute("resist_infection", 0, true)->value());
	skills[FASTING] = atoi(skillNode->first_attribute("fasting", 0, true)->value());
	skills[FORCED_INSOMNIA] = atoi(skillNode->first_attribute("forced_insomnia", 0, true)->value());
	skills[OBSERVER] = atoi(skillNode->first_attribute("observer", 0, true)->value());
	skills[SEARCH] = atoi(skillNode->first_attribute("search", 0, true)->value());
	skills[FISHING] = atoi(skillNode->first_attribute("fishing", 0, true)->value());
	skills[NAVIGATION] = atoi(skillNode->first_attribute("navigation", 0, true)->value());
	skills[TRACKING] = atoi(skillNode->first_attribute("tracking", 0, true)->value());
	skills[LOCKPICKING_PICKPOCKETING] = atoi(skillNode->first_attribute("lockpicking_and_pickpocketing", 0, true)->value());
	skills[DISGUISES] = atoi(skillNode->first_attribute("disguise", 0, true)->value());
	skills[CAMOUFLAGE] = atoi(skillNode->first_attribute("camouflage", 0, true)->value());
	skills[ARC_WEAPONS] = atoi(skillNode->first_attribute("arc_trajectory_weapons", 0, true)->value());
	skills[PRESERVE_FOOD] = atoi(skillNode->first_attribute("preserve_food", 0, true)->value());
	skills[MOTORCYCLE] = atoi(skillNode->first_attribute("pilot_motorcycle", 0, true)->value());
	skills[CAR] = atoi(skillNode->first_attribute("pilot_car", 0, true)->value());
	skills[CARGO_VEHICLE] = atoi(skillNode->first_attribute("pilot_cargo_vehicle", 0, true)->value());
	skills[HORSEMANSHIP] = atoi(skillNode->first_attribute("horsemanship", 0, true)->value());
	skills[STREETWISE] = atoi(skillNode->first_attribute("streetwise", 0, true)->value());
	skills[FIRST_AID] = atoi(skillNode->first_attribute("first_aid", 0, true)->value());
	skills[PURIFY_WATER] = atoi(skillNode->first_attribute("purify_water", 0, true)->value());
	skills[EXTINGUISH] = atoi(skillNode->first_attribute("extinguish", 0, true)->value());
	skills[PUBLIC_SPEAKING] = atoi(skillNode->first_attribute("public_speaking", 0, true)->value());
	skills[INSTRUCTION] = atoi(skillNode->first_attribute("instruction", 0, true)->value());
	skills[LEADERSHIP] = atoi(skillNode->first_attribute("leadership", 0, true)->value());
	skills[PERSUASION] = atoi(skillNode->first_attribute("persuasion", 0, true)->value());
	skills[MIMIC_VOICE] = atoi(skillNode->first_attribute("mimic_voice", 0, true)->value());
	skills[BARTER] = atoi(skillNode->first_attribute("barter", 0, true)->value());
	skills[RALLY] = atoi(skillNode->first_attribute("rally", 0, true)->value());
	skills[THREATEN] = atoi(skillNode->first_attribute("threaten", 0, true)->value());
	skills[BIOLOGY] = atoi(skillNode->first_attribute("biology", 0, true)->value());
	skills[CHEMISTRY] = atoi(skillNode->first_attribute("chemistry", 0, true)->value());
	skills[PROGRAMMING] = atoi(skillNode->first_attribute("programming", 0, true)->value());
	skills[GEOLOGY] = atoi(skillNode->first_attribute("geology", 0, true)->value());
	skills[MEDICINE] = atoi(skillNode->first_attribute("medicine", 0, true)->value());
	skills[BASIC_MECHANICS] = atoi(skillNode->first_attribute("basic_mechanics", 0, true)->value());
	skills[EXPERT_MECHANICS] = atoi(skillNode->first_attribute("expert_mechanics", 0, true)->value());
	skills[METEOROLOGY] = atoi(skillNode->first_attribute("meteorology", 0, true)->value());
	skills[METALLURGY] = atoi(skillNode->first_attribute("metallurgy", 0, true)->value());
	skills[BOTANY] = atoi(skillNode->first_attribute("botany", 0, true)->value());
	skills[MATHEMATICS] = atoi(skillNode->first_attribute("mathematics", 0, true)->value());
	skills[TANK_APC] = atoi(skillNode->first_attribute("pilot_tank_and_apc", 0, true)->value());
	skills[SMALL_BOAT] = atoi(skillNode->first_attribute("pilot_small_boat", 0, true)->value());
	skills[LARGE_BOAT] = atoi(skillNode->first_attribute("pilot_large_boat", 0, true)->value());
	skills[SHUTTLE] = atoi(skillNode->first_attribute("pilot_shuttle", 0, true)->value());
	skills[STARSHIP] = atoi(skillNode->first_attribute("pilot_starship", 0, true)->value());
	skills[LITERACY] = atoi(skillNode->first_attribute("literacy", 0, true)->value());
	skills[WRITING] = atoi(skillNode->first_attribute("writing", 0, true)->value());
	skills[EXPLOSIVES] = atoi(skillNode->first_attribute("explosives", 0, true)->value());
	skills[PHYSICS] = atoi(skillNode->first_attribute("physics", 0, true)->value());
	skills[ELECTRICAL_ENGINEERING] = atoi(skillNode->first_attribute("electrical_engineering", 0, true)->value());
	skills[CARPENTRY] = atoi(skillNode->first_attribute("carpentry", 0, true)->value());
}

void Actor::LoadStats(rapidxml::xml_node<>* statNode)
{
	if(!statNode)
		throw "Stats not referenced";

	stats[STRENGTH] = atoi(statNode->first_attribute("strength", 0, true)->value());
	stats[AGILITY] = atoi(statNode->first_attribute("agility", 0, true)->value());
	stats[STAMINA] = atoi(statNode->first_attribute("stamina", 0, true)->value());
	stats[INTELLIGENCE] = atoi(statNode->first_attribute("intelligence", 0, true)->value());
	stats[EDUCATION] = atoi(statNode->first_attribute("education", 0, true)->value());
	stats[CHARISMA] = atoi(statNode->first_attribute("charisma", 0, true)->value());
	stats[AURA] = atoi(statNode->first_attribute("aura", 0, true)->value());
}

void Actor::LoadHP(rapidxml::xml_node<>* hpNode)
{
	if(!hpNode)
		throw "HP not referenced";

	hp[RARM][0] = atoi(hpNode->first_attribute("right_arm_current", 0, true)->value());
	hp[RARM][1] = atoi(hpNode->first_attribute("right_arm_total", 0, true)->value());
	hp[LARM][0] = atoi(hpNode->first_attribute("left_arm_current", 0, true)->value());
	hp[LARM][1] = atoi(hpNode->first_attribute("left_arm_total", 0, true)->value());
	hp[RLEG][0] = atoi(hpNode->first_attribute("right_leg_current", 0, true)->value());
	hp[RLEG][1] = atoi(hpNode->first_attribute("right_leg_total", 0, true)->value());
	hp[LLEG][0] = atoi(hpNode->first_attribute("left_leg_current", 0, true)->value());
	hp[LLEG][1] = atoi(hpNode->first_attribute("left_leg_total", 0, true)->value());
	hp[TORSO][0] = atoi(hpNode->first_attribute("torso_current", 0, true)->value());
	hp[TORSO][1] = atoi(hpNode->first_attribute("torso_total", 0, true)->value());
	hp[HEAD][0] = atoi(hpNode->first_attribute("head_current", 0, true)->value());
	hp[HEAD][1] = atoi(hpNode->first_attribute("head_total", 0, true)->value());

	if(hp[HEAD][0] < 1 || hp[TORSO][0] < 1)
		alive = false;
	else
		alive = true;
}

void Actor::SetDest(int x, int y)
{
	dest.x = x;
	dest.y = y;
}

sf::Vector2i Actor::GetDest()
{
	return dest;
}

bool Actor::IsAnchored()
{
	return anchored;
}

void Actor::SetAnchor(bool anchor)
{
	anchored = anchor;
}
void Actor::Move(int x, int y)
{
	position.x += x;
	position.y += y;

	//AdvanceFrame();
}

void Actor::SetPosition(int x, int y)
{
	tilePosition.x = x;
	tilePosition.y = y;
}

void Actor::SetRealPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

int Actor::GetSpeed()
{
	return speed;
}

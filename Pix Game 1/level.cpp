#include <vector>
#include <string>
#include <fstream>
#include <iostream>  //debug
#include "level.h"
#include "tile.h"
#include "texturemanager.h"
#include "rapidxml\rapidxml.hpp"
#include "entity.h"
#include <SFML\Graphics.hpp>

Level::Level(int w, int h)
{
	SetDimensions(w, h);
	this->w = w;
	this->h = h;

	entCount = 0;

	selectedTile = NULL;
}

Level::~Level()
{

}

int Level::GetHeight()
{
	return h;
}

int Level::GetWidth()
{
	return w;
}

void Level::SetDimensions(int w, int h)
{
	//W rows
	map.resize(w);

	//Each row has h columns of NULL tile pointers
	for (int i = 0; i < w; i++)
	{
		map.at(i).resize(h, 0);
	}
}

void Level::AddTile(unsigned int x, unsigned int y, Tile* tile)
{
	map[x][y] = tile;
}

Tile* Level::GetTile(unsigned int x, unsigned int y)
{
	if(x < map.capacity() && y < map[x].capacity())
		return map[x][y];
	return NULL;
}

void Level::SelectTile(Tile* tile)
{
	if(tile)
	{
		selectedTile = tile;
	}
}

bool Level::IsSelectedTile(Tile* tile)
{
	return tile == selectedTile;
}

void Level::ClearSelect()
{
	selectedTile = NULL;
}

void Level::DrawSelectBorder(int x, int y, sf::RenderWindow* window)
{
	selectorSprite.setPosition(x, y);
	window->draw(selectorSprite);
}

void Level::HoverTile(Tile* tile)
{
	if(tile)
	{
		hoverTile = tile;
	}
	else
	{
		ClearHover();
	}
}

bool Level::IsHoveredTile(Tile* tile)
{
	return tile == hoverTile;
}

void Level::ClearHover()
{
	hoverTile = NULL;
}

void Level::DrawHoverBorder(int x, int y, sf::RenderWindow* window)
{
	hoverSprite.setPosition(x, y);
	window->draw(hoverSprite);
}

void Level::DrawGridSprite(int x, int y, sf::RenderWindow* window)
{
	gridSprite.setPosition(x, y);
	window->draw(gridSprite);
}

void Level::AddEntity(rapidxml::xml_node<>* entityXml)
{
	Entity* entity = new Entity();
	entity->LoadEntity(entityXml);
	int x = entity->GetPosition().x;
	int y = entity->GetPosition().y;
	Tile* occupied = GetTile(x, y);
	occupied->SetOccupant(entity);
	entities.resize(++entCount);
	entities[entCount - 1] = entity;
}

//TODO: get entity by location
//Entity* Level::GetEntity(int x, int y)
//{
//
//}

Entity* Level::GetEntity(int i)
{
	return entities[i];
}

void Level::LoadLevel(std::string filename)
{
	//load XML file
	std::ifstream inFile;
	inFile.open(filename);

	if(!inFile)
		throw "Could not load level";

	std::string xmlContents;
	std::string line;
	while(std::getline(inFile, line))
		xmlContents += line;

	//Convert string to RapidXML-readable char pointer
	std::vector<char> xmlData = std::vector<char>(xmlContents.begin(), xmlContents.end());
	xmlData.push_back('\0');

	//create parsed document with &XMLDATA[0], which is equialent to char pointer
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_no_data_nodes>(&xmlData[0]);

	//get root node
	rapidxml::xml_node<>* root = doc.first_node();

	//get level attributes
	int width = atoi(root->first_attribute("width")->value());
	int height = atoi(root->first_attribute("height")->value());

	//resize level
	this->w = width;
	this->h = height;
	SetDimensions(width, height);

	//Load each necessary tileset
	rapidxml::xml_node<>* tileset = root->first_node("tileset", 0, true);
	while(tileset)
	{
		std::string path = tileset->first_attribute("path", 0, true)->value();
		//load tileset
		textureManager.LoadTileset(path);
		//go to next tileset
		tileset = tileset->next_sibling("tileset", 0, true);
	}

	//Create selector sprite
	selectorSprite.setTexture(textureManager.GetTexture(4), true);

	//Create hover sprite
	hoverSprite.setTexture(textureManager.GetTexture(6), true);

	//Create grid sprite
	gridSprite.setTexture(textureManager.GetTexture(5), true);

	//go through each tile
	rapidxml::xml_node<>* tile = root->first_node("tile", 0, true);
	while(tile)
	{
		//get all attributes
		int x = atoi(tile->first_attribute("x", 0, true)->value());
		int y = atoi(tile->first_attribute("y", 0, true)->value());
		int baseid = atoi(tile->first_attribute("baseid", 0, true)->value());

		//check if tile is a wall or otherwise impassible
		bool wall = baseid == 1;

		//create tile and add to level
		Tile* newTile = new Tile(textureManager.GetTexture(baseid), wall);
		AddTile(x, y, newTile);

		//next tile
		tile = tile->next_sibling();
	}

	//load entities
	rapidxml::xml_node<>* entityXml = root->first_node("entity", 0, true);
	for( ; entityXml; entityXml = entityXml->next_sibling("entity", 0, true))
	{
		AddEntity(entityXml);
	}
}


void Level::UnloadLevel()
{
	for(unsigned int y = 0; y < map[0].capacity(); y++)
	{
		for(unsigned int x = 0; x < map.capacity(); x++)
		{
			delete map[x][y];
		}
	}

	for(unsigned int i = 0; i < entities.capacity(); i++)
		delete entities[i];
}
#include <vector>
#include <string>
#include <fstream>
#include <iostream>  //debug
#include <math.h>
#include <queue>
#include "level.h"
#include "tile.h"
#include "texturemanager.h"
#include "rapidxml\rapidxml.hpp"
#include "entity.h"
#include "tilenode.h"
#include <SFML\Graphics.hpp>

Level::Level(int w, int h)
{
	SetDimensions(w, h);
	this->w = w;
	this->h = h;

	entCount = 0;
	actCount = 0;

	//I have no idea what these values default to, but not setting them to NULL fucks everything up.
	selectedTile = NULL;
	selectedActor = NULL;
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

//Sets the dimensions of the level.
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

//Adds a tile to the level.
void Level::AddTile(unsigned int x, unsigned int y, Tile* tile)
{
	map[x][y] = tile;
}

//Returns the tile at (x, y).
Tile* Level::GetTile(unsigned int x, unsigned int y)
{
	if(x < map.capacity() && y < map[x].capacity())
		return map[x][y];
	return NULL;
}

//Selects a tile.
void Level::SelectTile(Tile* tile)
{
	if(tile)
	{
		selectedTile = tile;
	}
}

//Checks if a tile is the currently selected tile.
bool Level::IsSelectedTile(Tile* tile)
{
	return tile == selectedTile;
}

//Clears the selected tile.
void Level::ClearSelect()
{
	selectedTile = NULL;
}

//Draws the selection border sprite over the selected tile.
void Level::DrawSelectBorder(int x, int y, sf::RenderWindow* window)
{
	selectorSprite.setPosition(x, y);
	window->draw(selectorSprite);
}

//Returns the selected tile.
Tile* Level::GetSelectedTile()
{
	return selectedTile;
}

//Returns the selected actor.
Actor* Level::GetSelectedActor()
{
	return selectedActor;
}

//Sets the selected actor.
void Level::SetSelectedActor(Actor* actor)
{
	selectedActor = actor;
}

//Clears the selected actor.
void Level::ClearSelectedActor()
{
	selectedActor = NULL;
}

//Resets A*-related stats for all tiles.
void Level::ClearNodes()
{
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			GetTile(i, j)->ClearNode();
}

//Opens all tiles for exploration by A*.
void Level::OpenNodes()
{
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			GetTile(i, j)->SetClosed(false);
}

//Updates which tile is the hover tile, or clears it if the cursor is not over a tile.
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

//Determines whether a tile is being hovered.
bool Level::IsHoveredTile(Tile* tile)
{
	return tile == hoverTile;
}

//Clears the hover tile.
void Level::ClearHover()
{
	hoverTile = NULL;
}

//Draws the hover border over the hovered tile.
void Level::DrawHoverBorder(int x, int y, sf::RenderWindow* window)
{
	hoverSprite.setPosition(x, y);
	window->draw(hoverSprite);
}

//In Action Mode, draws one square of the grid over a base tile.
void Level::DrawGridSprite(int x, int y, sf::RenderWindow* window)
{
	gridSprite.setPosition(x, y);
	window->draw(gridSprite);
}

//For a tile, draws a green overlay if it is reachable, red if it is not, for indication to the player.
void Level::DrawMoveSprites(int x, int y, int speed, Tile* tile, sf::RenderWindow* window)
{
	if(tile->IsReachable(speed))
	{
		moveSprite.setPosition(x, y);
		window->draw(moveSprite);
	}
	else
	{
		noMoveSprite.setPosition(x, y);
		window->draw(noMoveSprite);
	}
}

//Adds an entity to the level from its XML file.
void Level::AddEntity(rapidxml::xml_node<>* root)
{
	Entity* entity = new Entity();
	entity->LoadEntity(root);
	int x = entity->GetPosition().x;
	int y = entity->GetPosition().y;
	Tile* occupied = GetTile(x, y);

	if(!occupied)
		throw "Entity placed on non-tile";

	occupied->SetOccupant(entity);
	entities.resize(++entCount);
	entities[entCount - 1] = entity;
}

//Returns an entity.
Entity* Level::GetEntity(int i)
{
	return entities[i];
}

//Adds an actor to the level from its XML file.
void Level::AddActor(rapidxml::xml_node<>* root)
{
	if(!root)
		throw "Actor XML missing";

	Actor* actor = new Actor();
	actor->LoadActor(root);
	int x = actor->GetPosition().x;
	int y = actor->GetPosition().y;
	Tile* occupied = GetTile(x, y);

	if(!occupied)
		throw "Actor placed on non-tile";

	occupied->SetDomOccupant(actor);
	actors.resize(++actCount);
	actors[actCount - 1] = actor;
}

//Returns an actor.
Actor* Level::GetActor(int i)
{
	return actors[i];
}

//Updates entities on the level based on changes in state from input.
void Level::Update()
{
	for(int i = 0; i < actCount; i++)
	{
		if(!actors[i])
			throw "Seems as though we lost an actor somewhere along the line.";
		UpdatePos(actors[i]);
	}
}

//Updates the position of an Actor's sprite, and gives the Actor its next step along the path to its destination.
void Level::UpdatePos(Actor* actor)
{
	if(!actor)
		throw "We seem to have lost an actor along the way.";

	int x = actor->GetPosition().x;
	int y = actor->GetPosition().y;

	//If anchored in a tile, find next tile on path
	if(actor->IsAnchored())
	{
		if(actor->GetDest() == actor->GetPosition())
		{
			actor->pathIdx = 0;
			return;
		}
		else if(GetTile(actor->GetDest().x, actor->GetDest().y)->IsReachable(actor->GetSpeed()))
		{
			actor->SetDir(GetTile(actor->GetDest().x, actor->GetDest().y)->GetPath()[actor->pathIdx]);
			actor->SetAnchor(false);
			actor->pathIdx++;
		}
		else
		{
			actor->SetDest(x, y);
			actor->pathIdx = 0;
		}
	}
	else
	{
		actor->Move(dx[actor->GetDir()], dy[actor->GetDir()]);

		if(abs(actor->GetRealPosition().x - x * TILESIZE) >= 40 ||
			abs(actor->GetRealPosition().y - y * TILESIZE) >= 40)
		{
			GetTile(x, y)->SetDomOccupant(NULL);
			actor->SetPosition(x + dx[actor->GetDir()], y + dy[actor->GetDir()]);
			GetTile(actor->GetPosition().x, actor->GetPosition().y)->SetDomOccupant(actor);
			actor->SetRealPosition(actor->GetPosition().x * TILESIZE, actor->GetPosition().y * TILESIZE);
			actor->SetAnchor(true);
		}
	}
}

//Determines whether an actor can reach a given tile.
bool Level::ReachTile(Actor* actor, Tile* tile)
{
	return tile->IsReachable(actor->GetSpeed());
}

//Runs A* on every tile on the edge of the speed radius, thus getting distances for all intervening tiles.
void Level::GetPaths(int x, int y, int speed)
{
	for(int i = 0; i <= speed + 1; i++)
	{
		//Find paths for tile in each quadrant, redundancies foregone
		if(GetTile(x + i, y + speed - i))
			if(GetTile(x + i, y + speed - i)->IsWalkable() && i != 0 && speed - i !=0)
			{
				GetTile(x + i, y + speed - i)->SetPath(Pathfind(x, y, x + i, y + speed - i, speed));
				OpenNodes();
			}
		if(GetTile(x + i, y - speed + i))
			if(GetTile(x + i, y - speed + i)->IsWalkable() && speed - i != 0)
			{
				GetTile(x + i, y - speed + i)->SetPath(Pathfind(x, y, x + i, y - speed + i, speed));
				OpenNodes();
			}
		if(GetTile(x - i, y - speed + i))
			if(GetTile(x - i, y - speed + i)->IsWalkable() && i != 0 && speed - i !=0)
			{
				GetTile(x - i, y - speed + i)->SetPath(Pathfind(x, y, x - i, y - speed + i, speed));
				OpenNodes();
			}
		if(GetTile(x - i, y + speed - i))
			if(GetTile(x - i, y + speed - i)->IsWalkable() && i != 0)
			{
				GetTile(x - i, y + speed - i)->SetPath(Pathfind(x, y, x - i, y + speed - i, speed));
				OpenNodes();
			}
	}
}

//A* between (x0, y0) and (x1, y1).  Stores the retraced path in the destination tile.
int* Level::Pathfind(int x0, int y0, int x1, int y1, int speed)
{
	priority_queue<TileNode> pq[2];
	int pqi;
	static TileNode* n0;
	static TileNode* m0;
	int x, y, xdx, ydy;
	int* path = new int[speed];

	pqi = 0;
	n0 = new TileNode(GetTile(x0, y0));

	n0->GetTile()->UpdateDist(0);
	n0->GetTile()->UpdatePriority(x1, y1);
	pq[pqi].push(*n0);

	while(!pq[pqi].empty())
	{
		n0 = new TileNode(pq[pqi].top().GetTile());

		x = n0->GetTile()->GetLoc().x;
		y = n0->GetTile()->GetLoc().y;

		pq[pqi].pop();

		if(x == x1 && y == y1)
		{
			if(GetTile(x, y)->GetDist() > speed)
				return NULL;
			for(int i = GetTile(x, y)->GetDist(); i > 0; i--)
			{
				for(int dir = 0; dir < 4; dir++)
				{
					//Reconstruct path backwards
					xdx = x - dx[dir];
					ydy = y - dy[dir];
					if(GetTile(xdx, ydy)->GetDist() < i)
						path[i - 1] = dir;
				}
				x = x - dx[path[i - 1]];
				y = y - dy[path[i - 1]];
			}
			while(!pq[pqi].empty())
				pq[pqi].pop();
			delete n0;
			return path;
		}

		for(int i = 0; i < 4; i++)
		{
			xdx = x + dx[i];
			ydy = y + dy[i];

			m0 = new TileNode(GetTile(xdx, ydy));
			if(m0)
				if(m0->GetTile()->IsWalkable() && !m0->GetTile()->IsClosed())
				{
					m0->GetTile()->UpdateDist(n0->GetTile()->GetDist() + 1);
					m0->GetTile()->UpdatePriority(x1, y1);
					if(m0->GetTile()->GetDist() <= speed)
						pq[pqi].push(*m0);
					else
						m0->GetTile()->SetClosed(true);
				}
				else
				{
					m0->GetTile()->SetClosed(true);
					delete m0;
				}
		}

		n0->GetTile()->SetClosed(true);
		delete n0;
	}
	return NULL;
}

//Clears all dynamically allocated memory associated with the level.
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

	for(unsigned int j = 0; j <actors.capacity(); j++)
		delete actors[j];
}

//Loads the level from an XML file, then does the same for all entities within the level.
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

	//Create move sprite
	moveSprite.setTexture(textureManager.GetTexture(7), true);

	//Create no-move sprite
	noMoveSprite.setTexture(textureManager.GetTexture(8), true);

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
		Tile* newTile = new Tile(textureManager.GetTexture(baseid), wall, x, y);
		AddTile(x, y, newTile);

		//next tile
		tile = tile->next_sibling();
	}

	//load actors
	rapidxml::xml_node<>* actorXml = root->first_node("actor", 0, true);
	for( ; actorXml; actorXml = actorXml->next_sibling("actor", 0, true))
	{
		//****************
		//Begin XML readin
		//****************

		//get XML filename
		std::string actorFilename = actorXml->first_attribute("path", 0, true)->value();
		//load XML file
		std::ifstream actorInFile;
		actorInFile.open(actorFilename);

		if(!actorInFile)
			throw "Could not load actor";

		std::string actorLine;
		std::string actorXmlContents;
		while(std::getline(actorInFile, actorLine))
			actorXmlContents += actorLine;

		//Convert string to RapidXML-readable char pointer
		std::vector<char> actorXmlData = std::vector<char>(actorXmlContents.begin(), actorXmlContents.end());
		actorXmlData.push_back('\0');

		//create parsed document with &XMLDATA[0], which is equialent to char pointer
		rapidxml::xml_document<> actorDoc;
		actorDoc.parse<rapidxml::parse_no_data_nodes>(&actorXmlData[0]);

		//get root node
		rapidxml::xml_node<>* actorRoot = actorDoc.first_node("actor", 0, true);

		//****************
		//End XML readin
		//****************

		if(actorRoot)
			AddActor(actorRoot);
	}

	//load entities
	rapidxml::xml_node<>* entityXml = root->first_node("entity", 0, true);
	for( ; entityXml; entityXml = entityXml->next_sibling("entity", 0, true))
	{
		//****************
		//Begin XML readin
		//****************

		//get XML filename
		std::string entityFilename = entityXml->first_attribute("path", 0, true)->value();
		//load XML file
		std::ifstream entityInFile;
		entityInFile.open(entityFilename);

		if(!entityInFile)
			throw "Could not load entity";

		std::string entityLine;
		std::string entityXmlContents;
		while(std::getline(entityInFile, entityLine))
			entityXmlContents += entityLine;

		//Convert string to RapidXML-readable char pointer
		std::vector<char> entityXmlData = std::vector<char>(entityXmlContents.begin(), entityXmlContents.end());
		entityXmlData.push_back('\0');

		//create parsed document with &XMLDATA[0], which is equialent to char pointer
		rapidxml::xml_document<> entityDoc;
		entityDoc.parse<rapidxml::parse_no_data_nodes>(&entityXmlData[0]);

		//get root node
		rapidxml::xml_node<>* entityRoot = entityDoc.first_node("entity", 0, true);

		//****************
		//End XML readin
		//****************
		if(entityRoot)
			AddEntity(entityRoot);
	}
}
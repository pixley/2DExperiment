#include "engine.h"
#include "tile.h"
#include "texturemanager.h"
#include "actor.h"
#include "entity.h"
#include <SFML\Graphics.hpp>
#include <string>

const int WINDOWWIDTH = 1280;
const int WINDOWHEIGHT = 720;

Engine::Engine()
{
	actionMode = false;
}

Engine::~Engine()
{

}

//void Engine::LoadTextures()
//{
//	sf::Texture sprite;
//	sprite.loadFromFile("sprite1.png", sf::IntRect(0, 0, TILESIZE / 2, TILESIZE));
//
//	textureManager.AddTexture(sprite, 0);
//	
//	sprite.loadFromFile("sprite2.png", sf::IntRect(0, 0, TILESIZE / 2, TILESIZE));
//
//	textureManager.AddTexture(sprite, 1);
//
//	//testTile = new Tile(textureManager.GetTexture(0));
//}

bool Engine::Init()
{
	//Create window
	window = new sf::RenderWindow(sf::VideoMode(WINDOWWIDTH,WINDOWHEIGHT,32), "Anarchy Space RPG Prototype");
	if (!window)
		return false;
	window->setFramerateLimit(60);

	//Load level
	currentLevel = new Level(0, 0);
	std::string filename("level.xml");
	currentLevel->LoadLevel(filename);

	//Set up camera
	camera = new Camera(WINDOWWIDTH, WINDOWHEIGHT, 0.1f, currentLevel);
	camera->MoveCenter(400, 400);

	return true;
}

void Engine::RenderFrame()
{
	//camera offsets
	int camOffsetX, camOffsetY;
	
	//Variables: X & Y - how many tiles drawn, TILEX & TILEY - which tile being drawn
	int x, y, tileX, tileY;

	//Get tile bounds needed to draw and camera bounds
	sf::IntRect bounds = camera->GetTileBounds(TILESIZE);

	//tile pointer
	Tile* tile;

	window->clear(sf::Color::Black);

	//Figure out how much to offset each tile
	sf::Vector2i camOffset = camera->GetTileOffset(TILESIZE);
	camOffsetX = camOffset.x;
	camOffsetY = camOffset.y;

	//Loop and draw each tile
	for (y = 0, tileY = bounds.top; y < bounds.height; y++, tileY++)
	{
		for (x = 0, tileX = bounds.left; x < bounds.width; x++, tileX++)
		{
			//get the tile being drawn
			tile = currentLevel->GetTile(tileX, tileY);
			if(tile)
			{
				tile->Draw((x * TILESIZE) - camOffsetX, (y * TILESIZE) - camOffsetY, window);

				if(currentLevel->IsSelectedTile(tile))
				{
					currentLevel->DrawSelectBorder((x * TILESIZE) - camOffsetX, (y * TILESIZE) - camOffsetY, window);
				}
				if(currentLevel->IsHoveredTile(tile) && actionMode)
				{
					currentLevel->DrawHoverBorder((x * TILESIZE) - camOffsetX, (y * TILESIZE) - camOffsetY, window);
				}
				if(actionMode && currentLevel->GetSelectedActor())
				{
					currentLevel->DrawMoveSprites((x * TILESIZE) - camOffsetX, (y * TILESIZE) - camOffsetY, currentLevel->GetSelectedActor()->GetSpeed(), tile, window);
				}
				else if(actionMode)
				{
					currentLevel->DrawGridSprite((x * TILESIZE) - camOffsetX, (y * TILESIZE) - camOffsetY, window);
				}
			}
		}
	}

	for (y = 0, tileY = bounds.top; y < bounds.height; y++, tileY++)
	{
		for (x = 0, tileX = bounds.left; x < bounds.width; x++, tileX++)
		{
			//get the tile being drawn
			tile = currentLevel->GetTile(tileX, tileY);
			if(tile)
			{
				Entity* occupant = tile->GetOccupant();
				if (occupant)
				{
					occupant->Draw(window, camera->GetRealPosition().x, camera->GetRealPosition().y);
				}
	            
				Actor* domOccupant = tile->GetDomOccupant();
				if(domOccupant)
				{
					domOccupant->Draw(window, camera->GetRealPosition().x, camera->GetRealPosition().y);
				}
			}
		}
	}

	window->display();
}

void Engine::ProcessInput()
{
	sf::Event evt;
	//Loop through all window events
	while(window->pollEvent(evt))
	{
		if(evt.type == sf::Event::Closed)
			window->close();
		
		if((evt.type == sf::Event::MouseButtonPressed) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && actionMode)
		{
			int x = camera->GetPosition().x + sf::Mouse::getPosition(*window).x;
			int y = camera->GetPosition().y + sf::Mouse::getPosition(*window).y;
			
			Tile* tile = currentLevel->GetTile(x / TILESIZE, y / TILESIZE);
			currentLevel->SelectTile(tile);
			camera->GoToCenter(x , y);
		}

		if(evt.type == sf::Event::MouseMoved)
		{
			int x = camera->GetPosition().x + sf::Mouse::getPosition(*window).x;
			int y = camera->GetPosition().y + sf::Mouse::getPosition(*window).y;
			
			Tile* tile = currentLevel->GetTile(x / TILESIZE, y / TILESIZE);
			currentLevel->HoverTile(tile);
		}

		if((evt.type == sf::Event::KeyPressed) && sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			if(!actionMode)
			{
				actionMode = true;
				currentLevel->ClearNodes();
			}
			else
			{
				actionMode = false;
				currentLevel->ClearSelect();
				currentLevel->ClearSelectedActor();
			}
		}

		if((evt.type == sf::Event::KeyPressed) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			Tile* selectedTile = currentLevel->GetSelectedTile();
			Actor* selectedActor = currentLevel->GetSelectedActor();
			if(actionMode && selectedTile && selectedActor && selectedTile->IsReachable(selectedActor->GetSpeed()))
			{
				int x0 = selectedActor->GetPosition().x;
				int y0 = selectedActor->GetPosition().y;
				int x1 = selectedTile->GetLoc().x;
				int y1 = selectedTile->GetLoc().y;
				selectedTile->SetPath(currentLevel->Pathfind(x0, y0, x1, y1, selectedActor->GetSpeed()));
				selectedActor->SetDest(x1, y1);
				currentLevel->ClearSelect();
				currentLevel->ClearSelectedActor();
				actionMode = false;
			} else if (actionMode && selectedTile)
			{
				selectedActor = selectedTile->GetDomOccupant();
				if(selectedActor)
				{
					currentLevel->SetSelectedActor(selectedActor);
					currentLevel->ClearNodes();
					currentLevel->GetPaths(selectedActor->GetPosition().x, selectedActor->GetPosition().y, selectedActor->GetSpeed());
				}
			}
		}

		if((evt.type == sf::Event::KeyPressed) && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			currentLevel->ClearSelectedActor();
		}
	}
}

void Engine::Update()
{
	camera->Update();
	if(!actionMode)
		currentLevel->Update();
}

void Engine::MainLoop()
{
	//Loop until the window is closed
	while(window->isOpen())
	{
		RenderFrame();
		ProcessInput();
		Update();
	}
}

void Engine::Go()
{
	if(!Init())
		throw "Could not initialize Engine";

	//Run game
	MainLoop();

	//Clearing dynamic memory
	currentLevel->UnloadLevel();
	delete currentLevel;
	delete camera;
	delete window;
}

/*
void Engine::LoadLevel()
{
	//Temporary testing level
	currentLevel = new Level(40, 40);

	Tile* tile;
	for(int y = 0; y < 40; y++)
	{
		for(int x = 0; x < 40; x++)
		{
			if(y % 4 == 0)
				tile = new Tile(textureManager.GetTexture(1));
			else
				tile = new Tile(textureManager.GetTexture(0));

			currentLevel->AddTile(x, y, tile);
		}
	}
	camera = new Camera(800, 600, 1.0f);
	camera->Move(0, 0);
}
*/

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
	//Clearing dynamic memory
	delete currentLevel;
	delete camera;
	delete window;
}

bool Engine::Init()
{
	//Create window
	window = new sf::RenderWindow(sf::VideoMode(WINDOWWIDTH,WINDOWHEIGHT,32), "Anarchy Prototype");
	if (!window)
		return false;
	window->setFramerateLimit(60);

	//Load level
	std::string filename("level.xml");
	currentLevel = new Level(filename);

	//Set up camera
	camera = new Camera(WINDOWWIDTH, WINDOWHEIGHT, 0.1f, currentLevel);
	camera->MoveCenter(400, 400);

	return true;
}

void Engine::RenderFrame()
{
	//Get tile bounds needed to draw and camera bounds
	sf::IntRect bounds = camera->TileBounds(TILESIZE);

	window->clear(sf::Color::Black);

	//Figure out how much to offset each tile
	sf::Vector2i camOffset = camera->TileOffset(TILESIZE);

	currentLevel->Draw(actionMode, camOffset, bounds, camera->GetRealPosition(), window);

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
			
			if (actionMode)
				currentLevel->SelectTile(x / TILESIZE, y / TILESIZE);

			camera->GoToCenter(x , y);
		}

		if((evt.type == sf::Event::MouseButtonPressed) && sf::Mouse::isButtonPressed(sf::Mouse::Right) && actionMode)
		{
			if (actionMode) {
				int x = camera->GetPosition().x + sf::Mouse::getPosition(*window).x;
				int y = camera->GetPosition().y + sf::Mouse::getPosition(*window).y;

				currentLevel->TargetTile(x / TILESIZE, y / TILESIZE);
			}
		}

		if(evt.type == sf::Event::MouseMoved)
		{
			int x = camera->GetPosition().x + sf::Mouse::getPosition(*window).x;
			int y = camera->GetPosition().y + sf::Mouse::getPosition(*window).y;
			
			currentLevel->HoverTile(x / TILESIZE, y / TILESIZE);
		}

		if((evt.type == sf::Event::KeyPressed) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			actionMode = currentLevel->SwitchMode(actionMode);
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
}

#ifndef _ENGINE_H
#define _ENGINE_H

#include <SFML\Graphics.hpp>
#include "texturemanager.h"
#include "tile.h"
#include "camera.h"
#include "level.h"

/*
The game engine itself.  Handles display of game elements by way of calls to Level::Draw methods,
updating of entities and the camera, and human input.
*/
class Engine
{
private:
	//SFML Render Window
	sf::RenderWindow* window;

	//Camera
	Camera* camera;

	//Current level
	Level* currentLevel;

	//Indicates whether Action Mode is engaged
	bool actionMode;

	//Initializes the engine
	bool Init();
	//Main game loop
	void MainLoop();
	//Renders one frame
	void RenderFrame();
	//Processes user input
	void ProcessInput();
	//Updates all Engine internals
	void Update();

	//Loads the test level
	//void LoadLevel();

public:
	Engine();
	~Engine();

	void Go();
	//Starts the engine
};

#endif

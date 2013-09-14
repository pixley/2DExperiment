#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>
#include "rapidxml\rapidxml.hpp"
using namespace std;

const int TILESIZE = 40;

/*
Used to manage textures for both the level's tileset and for each entity by providing
an interface with SFML graphical libraries.
*/
class TextureManager
{
private:
	vector<sf::Texture> textureList;
	map<int, int> textureIDs;

public:
	TextureManager();
	~TextureManager();

	void AddTexture(sf::Texture& texture, int id);
	sf::Texture& GetTexture(int id);

	//loads tileset from XML file
	void LoadTileset(string filename);

	//loads spritesheet from XML file
	void LoadSpriteSheet(rapidxml::xml_node<>* spritesheet, int width, int height);
};

#endif
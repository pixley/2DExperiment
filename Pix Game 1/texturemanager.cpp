#include "texturemanager.h"
#include <vector>
#include <fstream>
#include <iostream>  //debug
#include <SFML\Graphics.hpp>
#include "rapidxml\rapidxml.hpp"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

void TextureManager::AddTexture(sf::Texture& texture, int id)
{
	textureList.push_back(texture);

	//Map for pairing texture IDs and their index and TEXTURE LIST
	textureIDs[id] = textureList.size() - 1;
}

sf::Texture& TextureManager::GetTexture(int id)
{
	return textureList[textureIDs[id]];
}

void TextureManager::LoadTileset(std::string filename)
{
	//Load file
	std::ifstream inFile(filename);

	if(!inFile)
		throw "Could not find tileset";

	//Dump contents of file into a string
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
	rapidxml::xml_node<>* root = doc.first_node("tileset", 0 ,true);

	std::string texturePath;
	sf::Texture tileset;

	//Go through each texture file
	rapidxml::xml_node<>* texturefile = root->first_node("texturefile", 0, true);
	while(texturefile)
	{
		//get texture file being parsed and load it
		texturePath = texturefile->first_attribute("path", 0, true)->value();
		tileset.loadFromFile(texturePath);
		sf::Image textureImage = tileset.copyToImage();

		//Go through each tile
		rapidxml::xml_node<>* tile = texturefile->first_node("tile", 0, true);
		while(tile)
		{
			//get all attributes
			int x = atoi(tile->first_attribute("x", 0, true)->value());
			int y = atoi(tile->first_attribute("y", 0, true)->value());
			int frames = atoi(tile->first_attribute("frames", 0, true)->value());
			int id = atoi(tile->first_attribute("id", 0, true)->value());

			//copy correct texture from tileset
			sf::Texture tileTexture;
			tileTexture.create(TILESIZE, TILESIZE);
			tileTexture.loadFromImage(textureImage, sf::IntRect(x * TILESIZE, y * TILESIZE, frames * TILESIZE, TILESIZE));

			//Add texture to texture list
			AddTexture(tileTexture, id);

			//Next tile
			tile = tile->next_sibling();
		}

		//Next texture file
		texturefile = texturefile->next_sibling();
	}
}

int TextureManager::LoadSpriteSheet(rapidxml::xml_node<>* spritesheet, int width, int height)
{
	//get texture file being parsed and load it
	std::string texturePath;
	sf::Texture sheet;
	texturePath = spritesheet->first_attribute("path", 0, true)->value();
	sheet.create(width, height);

	if(texturePath.empty())
		throw "Invalid texture path";

	sheet.loadFromFile(texturePath);
	
	//get attributes
	int id = atoi(spritesheet->first_attribute("id", 0, true)->value());

	//Add texture to texture list
	AddTexture(sheet, id);

	return id;
}

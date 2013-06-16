#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include <vector>
#include <SFML\Graphics.hpp>

class TextureManager
{
private:
	vector<sf::Texture> imageList;

public:
	TextureManager();
	~TextureManager();

	void AddTexture(sf::Texture& texture);
	sf::Texture& GetTexture(int index);
}

#endif
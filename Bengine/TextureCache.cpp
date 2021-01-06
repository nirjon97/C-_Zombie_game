#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace Bengine{
	TextureCache::TextureCache(void)
	{
	}


	TextureCache::~TextureCache(void)
	{
	}
	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//lookup the texture and see if its in the map
		//std::map<std::string,GLTexture>::iterator
		auto mit=_textureMap.find(texturePath);

		//check if the texture is not in the map
		if(mit== _textureMap.end())
		{
			//Load the texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			//Insert into map
			//std::pair<std::string,GLTexture> newPair(texturePath,newTexture);
			_textureMap.insert(make_pair(texturePath,newTexture));

	//std::cout<<"Used Cache texture\n";
			return newTexture;
		}
		return mit->second;

	}
}
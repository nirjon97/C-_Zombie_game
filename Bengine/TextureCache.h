#pragma once
#include <map>
#include "GLTextures.h"
namespace Bengine{
	class TextureCache
	{
	public:
		TextureCache(void);
		~TextureCache(void);

		GLTexture getTexture(std::string texturePath);
	private:
		std::map<std::string,GLTexture> _textureMap;
	};
}

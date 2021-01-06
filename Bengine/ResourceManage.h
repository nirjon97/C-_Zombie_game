#pragma once
#include "TextureCache.h"
#include <string>
namespace Bengine{
	class ResourceManage
	{
	public:
		static GLTexture getTexture(std::string texturePath);


	private:
		static TextureCache _textureCache;

	};
}


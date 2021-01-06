#include "ResourceManage.h"

namespace Bengine {
	TextureCache ResourceManage::_textureCache;

	GLTexture  ResourceManage::getTexture(std::string texturePath)
	{
		return _textureCache.getTexture(texturePath);
	}

}
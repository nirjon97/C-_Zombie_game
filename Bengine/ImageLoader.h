#pragma once
#include "GLTextures.h"
#include <string>
namespace Bengine{
	 class  ImageLoader
	{
		public:
		static GLTexture loadPNG(std::string filePath);
	
	};
}

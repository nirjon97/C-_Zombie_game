#pragma once
#include <GL\glew.h>
#include "GLTextures.h"
#include <string>
namespace Bengine{
	class Sprite
	{
	public:
		Sprite(void);
		~Sprite(void);

		void init(float x,float y,float width,float height,std::string texturePath);
		void draw();
	private:
		float _x;
		float _y;
		float _width;
		float _height;
		//vertex buffer object id
		GLuint _vboID;
		GLTexture _texture;
	};
}

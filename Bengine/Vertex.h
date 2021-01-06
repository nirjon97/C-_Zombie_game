#pragma once

#include <GL\glew.h>
namespace Bengine{
	struct Position{
		float x;
		float y;
	};
	struct ColorRGBA8{
		ColorRGBA8() :red(0), green(0), blue(0), alpha(0) { }
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
			red(R), green(G), blue(B), alpha(A)   { }
	
		GLubyte red;
		GLubyte green;
		GLubyte blue;
		GLubyte alpha;
	};
	struct UV{
		float u;
		float v;
	};
	struct Vertex{
		Position position;
		ColorRGBA8 color;

		//UV texture coordinate for texture mapping
		UV uv;

		void setPosition(float x, float y){
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r,GLubyte g,GLubyte b,GLubyte a){
			color.red =r;
			color.green = g;
			color.blue = b;
			color.alpha = a;
		}

		void setUV(float u, float v){
			uv.u =u;
			uv.v=v;
		}

	};
}
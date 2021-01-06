#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManage.h"
#include <stddef.h>

namespace Bengine{
	Sprite::Sprite(void): _vboID(0)
	{

	}


	Sprite::~Sprite(void)
	{
		//Free buffer
		if(_vboID !=0){
			glDeleteBuffers(1,&_vboID);
		}
	}

	void Sprite::init(float x,float y,float width,float height,std::string texturePath)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManage::getTexture(texturePath);

		if(_vboID == 0)
		{
			glGenBuffers(1,&_vboID);
		}
		//Make an array of coordinate vertex of the square-combined by 2 triangle
		//the array will have a size 6*2=12-6 vertice with x,y coordinate
		Vertex vertexData[6];
		//First triangle
		vertexData[0].setPosition(x+width,y+height);
		vertexData[0].setUV(1.0f,1.0f);

		vertexData[1].setPosition(x,y+height);
		vertexData[1].setUV(0.0f,1.0f);

		vertexData[2].setPosition(x,y);
		vertexData[2].setUV(0.0f,0.0f);

		//Second triangle
		vertexData[3].setPosition(x,y);
		vertexData[3].setUV(0.0f,0.0f);

		vertexData[4].setPosition(x+width,y);
		vertexData[4].setUV(1.0f,0.0f);

		vertexData[5].setPosition(x+width,y+height);
		vertexData[5].setUV(1.0f,1.0f);


		//Set color for vertex
		for(int i=0;i<6;i++)
		{
			vertexData[i].setColor(255,0,255,255);

		}
		vertexData[1].setColor(0,0,255,255);

		vertexData[4].setColor(0,255,0,255);

		//Create and upload buffer data from CPU to GPU for draw
		glBindBuffer(GL_ARRAY_BUFFER,_vboID);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW);

		//Undind buffer
		glBindBuffer(GL_ARRAY_BUFFER,0);


	}
	//We will create vertex buffer object-contains array of vertice in cpu and upload it to GPU for drawing
	void Sprite::draw()
	{
		//Bind texture 
		glBindTexture(GL_TEXTURE_2D,_texture.id);
		//Draw the vertex base on the vertice in init function
		glBindBuffer(GL_ARRAY_BUFFER,_vboID);

		//Enable 1 vertex object in gpu
		glEnableVertexAttribArray(0);

		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(2);


		//Tell open GL where the start of coordinate data-set up the vertice coordinate 
		//This is the position attribute pointer 
		glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,position));
		//This is the color attributes pointer
		glVertexAttribPointer(1,4,GL_UNSIGNED_BYTE,GL_TRUE,sizeof(Vertex),(void*)offsetof(Vertex,color));
		//This is the uv attributes pointer
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,uv));

		//Draw the triangel base on the vertice coordinate above
		glDrawArrays(GL_TRIANGLES,0,6);

		//Unbind buffer after draw
		glDisableVertexAttribArray(0);

		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER,0);

	}
}
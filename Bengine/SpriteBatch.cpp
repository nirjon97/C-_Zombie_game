#include "SpriteBatch.h"
#include <algorithm>

namespace Bengine{


	SpriteBatch::SpriteBatch(void): _vbo(0),_vao(0)
	{
	}


	SpriteBatch::~SpriteBatch(void)
	{
	}


	void SpriteBatch::init()
	{
		createVertexArray();
	}



	void SpriteBatch::begin(GlyphSortType sortType/*GlyphSortType::TEXTURE by default*/)
	{
		_sortType = sortType;
		_renderBatches.clear();
		//Free the glyphs pointer
		for(int i=0;i<_glyphs.size();i++)
		{
			delete _glyphs[i];
		}
		_glyphs.clear();

	}

	void SpriteBatch::draw(const glm::vec4 &destRect,const glm::vec4 &uvRect,GLuint texture,float depth,const ColorRGBA8 &color)
	{
		Glyph* newGlyph=new Glyph();
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		//destRect.z-the width,destRect.w-the height
		//Set for vertice topleft
		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x,destRect.y+destRect.w);
		newGlyph->topLeft.setUV(uvRect.x,uvRect.y+uvRect.w);

		//Set for vertice bottomleft
		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x,destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x,uvRect.y);

		//Set vertice for bottomright
		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x+destRect.z,destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x+uvRect.z,uvRect.y);

		//Set vertice for topright
		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z,destRect.y+destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z,uvRect.y+uvRect.w);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle) {

	}

	void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, const glm::vec3 dir) {

	}

	void SpriteBatch::end()
	{
		sortGlyph();
		createRenderBatches();
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);
		for(GLuint i=0;i<_renderBatches.size();i++)
		{
			glBindTexture(GL_TEXTURE_2D,_renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES,_renderBatches[i].offset,_renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}
	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(_glyphs.size()*6);

		if(_glyphs.empty())
			return;
		RenderBatch myBatch(0,6,_glyphs[0]->texture); 
		_renderBatches.emplace_back(myBatch);
		//_renderBatches.emplace_back(0,6,_glyphs[0]->texture);

		int offset =0;
		int cv =0;//current vertex
		vertices[cv++] = _glyphs[0]->topLeft;
		vertices[cv++] = _glyphs[0]->bottomLeft;
		vertices[cv++] = _glyphs[0]->bottomRight;
		vertices[cv++] = _glyphs[0]->bottomRight;
		vertices[cv++] = _glyphs[0]->topRight;
		vertices[cv++] = _glyphs[0]->topLeft;
		//Make new vertices add offset
		offset +=6;
		for(int cg=1;cg < _glyphs.size();cg++)
		{
			if(_glyphs[cg]->texture !=_glyphs[cg-1]->texture)
			{
				//Different texture, create new batch
				RenderBatch mb(offset,6,_glyphs[cg]->texture); 
				_renderBatches.emplace_back(mb);
			}
			else
			{
				_renderBatches.back().numVertices +=6;
			}
			vertices[cv++] = _glyphs[cg]->topLeft;
			vertices[cv++] = _glyphs[cg]->bottomLeft;
			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->topRight;
			vertices[cv++] = _glyphs[cg]->topLeft;
			offset+=6;
		}
		//Upload vertex data to vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER,_vbo);
		//orphan the buffer to upload data
		glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),nullptr,GL_DYNAMIC_DRAW);
		//upload data
		glBufferSubData(GL_ARRAY_BUFFER,0,vertices.size()*sizeof(Vertex),vertices.data());

		//Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}
	//Vertex array object hold the state of each time in drawing a sprite in each frame
	void SpriteBatch::createVertexArray()
	{
		if(_vao==0)
			glGenVertexArrays(1,&_vao);

		glBindVertexArray(_vao);

		if(_vbo==0)
			glGenBuffers(1,&_vbo);

		glBindBuffer(GL_ARRAY_BUFFER,_vbo);

		//Enable 1 vertex object in gpu, tell Opengl
		//that we want to use 3 attribute array
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

		//Unbind all vbo
		glBindVertexArray(0);

	}
	void SpriteBatch::sortGlyph()
	{
		switch(_sortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(),_glyphs.end(),compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(),_glyphs.end(),compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(),_glyphs.end(),compareTexture);
			break;
		
		}

	}
	bool SpriteBatch::compareFrontToBack(Glyph *a,Glyph *b )
	{
		return (a->depth<b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph *a,Glyph *b )
	{
		return (a->depth>b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph *a,Glyph *b )
	{
		return (a->texture<b->texture);
	}
}

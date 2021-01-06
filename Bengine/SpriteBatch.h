#pragma once
#include <GL\glew.h>
#include "Vertex.h"
#include <glm\glm.hpp>
#include <vector>
namespace Bengine{
	enum  GlyphSortType{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE // SORT BY TEXTURE 
	};
	struct Glyph{

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex bottomRight;
		Vertex topRight;

	};
	class RenderBatch{
		public:
			RenderBatch(GLuint Offset,GLuint NumVertices,GLuint Textures):offset(Offset),
				numVertices(NumVertices),texture(Textures){}
			GLuint offset;
			GLuint numVertices;
			GLuint texture;	
	};
	class SpriteBatch
	{
	public:
		SpriteBatch(void);
		~SpriteBatch(void);

		//Init the sprite batch
		void init();

		//Setup any state we need to begin rendering, sset sort type of glyph
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		//Add to the batch for draw
		//Use reference parameter to prevent copy by value too munch
		void draw(const glm::vec4 &destRect,const glm::vec4 &uvRect,GLuint texture,float depth,const ColorRGBA8 &color);

		void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color,float angle);

		void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color,const glm::vec3 dir);
		//pause processing, sort all the different imag
		void end();

		//Finish and draw all sprite on the screen
		void renderBatch();
	private:
		GLuint _vbo;
		GLuint _vao;
		std::vector<Glyph*> _glyphs; 
		std::vector<RenderBatch> _renderBatches;
		GlyphSortType _sortType;

		void createRenderBatches();
		void createVertexArray();
		void sortGlyph();
		static bool compareFrontToBack(Glyph *a,Glyph *b );
		static bool compareBackToFront(Glyph *a,Glyph *b );
		static bool compareTexture(Glyph *a,Glyph *b );
	};
}


#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Bengine{
	class Camera2d
	{
	public:
		Camera2d(void);
		~Camera2d(void);	

		void init(int screenWidth,int screenHeight);
		void update();

		bool isBoxInView(const glm::vec2& Position, const glm::vec2& dimensions);

		//setter
		void setPosition(const glm::vec2 &newPosition){_position=newPosition;_needMatrixUpdate = true;}
		void setScale(float newScale){_scale = newScale;_needMatrixUpdate = true;}

		//getter
		glm::vec2 getPosition(){return _position;}
		float getScale(){return _scale;}
		glm::mat4 getCameraMatrix(){return _cameraMatrix;}

		//convert screen coordinate to game coordinate
		glm::vec2 convertScreentoWorld(glm::vec2 screenCoords);

	private:
		int _screenWidth,_screenHeight;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
		bool _needMatrixUpdate;
	};

}
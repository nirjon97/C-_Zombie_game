#include "Camera2d.h"

namespace Bengine{
	Camera2d::Camera2d(void): 
	_position(0.0f,0.0f), 
	_cameraMatrix(1.0f),
	_orthoMatrix(1.0f),
	_scale(1.0f),
	_needMatrixUpdate(true),
	_screenWidth(500),
	_screenHeight(500)
	{

	}


	Camera2d::~Camera2d(void)
	{
	}

	void Camera2d::init(int screenWidth,int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f,(float)_screenWidth,0.0f,(float)_screenHeight);
	}

	void Camera2d::update()
	{
		if(_needMatrixUpdate){
			//translate matrix to moving object in screen
			//Camera translation
			glm::vec3 translate(-_position.x + _screenWidth/2,-_position.y + _screenHeight/2,0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix,translate);


			//Camera scale
			//Scale the matrix for zoom in out-ex:*1.0 is mean nothing happen,*0.5 mean zoom dowwn, *2 zoom in
			glm::vec3 scale(_scale,_scale,0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f),scale) * _cameraMatrix;
			
			_needMatrixUpdate = false;
		}
   }
	
	glm::vec2 Camera2d::convertScreentoWorld(glm::vec2 screenCoords)
	{
		//Inverse y direction 
		screenCoords.y = _screenHeight - screenCoords.y;
		//Make the center of screen have (0,0) coordinate
		screenCoords -= glm::vec2(_screenWidth/2,_screenHeight/2);
		//The coordinate change base on the scale - zoomin smaller coordinate, zoom out bigger coordinate
		screenCoords /=_scale;
		//Translate with the camera position
		screenCoords += _position;
		return screenCoords;

	}

	bool Camera2d::isBoxInView(const glm::vec2& Position, const glm::vec2& dimensions) {

		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);

		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		glm::vec2 centerpos = _position + dimensions / 2.0f;

		glm::vec2 centercamerapos = _position;

		glm::vec2 distVec = centerpos - centercamerapos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//if this is true, we are colliding
		if (xDepth > 0 || yDepth > 0) {

			//there was a collision
			return true;
		}
		return false;
	}

}
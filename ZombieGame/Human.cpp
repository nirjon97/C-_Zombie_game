#include "Human.h"
#include<ctime>
#include<random>
#include <glm/gtx/rotate_vector.hpp>
#include <Bengine\ResourceManage.h>



Human::Human():
_frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position) {

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);

	_health = 20;

	_color.red = 255;
	_color.green = 255;
	_color.blue = 255;
	_color.alpha = 255;

	_speed = speed;
	_position = position;
	//get random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	//make sure direction isnot zero
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);

	_textureID = Bengine::ResourceManage::getTexture("Textures/walk3.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>&humans,
	std::vector<Zombie*>&zombies, float deltaTime)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);

	_position += _direction * _speed * deltaTime;

	//randomly changed direction every 20 frames
	if (_frames == 20) {

		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else {
		_frames++;
	}
	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}
}

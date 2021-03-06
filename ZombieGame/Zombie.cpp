#include "Zombie.h"
#include "Human.h"
#include <Bengine\ResourceManage.h>


Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}
void Zombie::init(float speed, glm::vec2 position) {
	_speed = speed;
	_position = position;
	_health = 50;

	_color = Bengine::ColorRGBA8(255, 255, 255, 255);
	_textureID = Bengine::ResourceManage::getTexture("Textures/3.png").id;
}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>&humans,
	std::vector<Zombie*>&zombies, float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction * _speed * deltaTime;
	}

	collideWithLevel(levelData);
}

Human*Zombie::getNearestHuman(std::vector<Human*>&humans){
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;

	for (int i = 0; i < humans.size(); i++){
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}

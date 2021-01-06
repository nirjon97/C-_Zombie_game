#pragma once

#include "Human.h"
#include "Bullet.h"
#include <Bengine\InputManager.h>
#include <Bengine\Camera2d.h>

class Gun;


class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, Bengine::InputManager* inputManager, Bengine::Camera2d* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>&humans,
		std::vector<Zombie*>&zombies, float deltaTime) override;
		

private:
	Bengine::InputManager* _inputManager;

	std::vector<Gun*> _guns;
	int _currentGunIndex;

	Bengine::Camera2d* _camera;
	std::vector<Bullet>* _bullets;

};


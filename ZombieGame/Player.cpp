#include "Player.h"

#include <SDL\SDL.h>
#include <Bengine\ResourceManage.h>


#include"Gun.h"

Player::Player() :
_currentGunIndex(-1)
{
	//empty
}


Player::~Player()
{
	//empty
}
void Player::init(float speed, glm::vec2 position, Bengine::InputManager* inputManager, Bengine::Camera2d* camera, std::vector<Bullet>* bullets)
{
	_inputManager = inputManager;
	_speed = speed;
	_camera = camera;
	_bullets = bullets;
	_position = position;
	_color.red = 255;
	_color.green = 255;
	_color.blue = 255;
	_color.alpha = 255;
	_health = 150;
	_textureID = Bengine::ResourceManage::getTexture("Textures/Hurt_007.png").id;
}

void Player::addGun(Gun* gun) {
	_guns.push_back(gun);

	if (_currentGunIndex == -1) {
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>&humans,
	std::vector<Zombie*>&zombies, float deltaTime)
{
	if (_inputManager->isKeyDown(SDLK_w))
	{
		_position.y += _speed * deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_s))
	{
		_position.y -= _speed *deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_a))
	{
		_position.x -= _speed* deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_d))
	{
		_position.x += _speed *deltaTime;
	}

	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0){
		_currentGunIndex = 0;
	}
	else if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1) {
		_currentGunIndex = 1;
	}
	else if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2) {
		_currentGunIndex = 2;
	}

	if (_currentGunIndex != -1) {

		glm::vec2 mouseCoords   = _inputManager->getMouseCoords();
		mouseCoords = _camera->convertScreentoWorld(mouseCoords);

		glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

		_guns[_currentGunIndex]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT),
			centerPosition,
			direction,
			*_bullets,
			deltaTime);
	}

	collideWithLevel(levelData);
}
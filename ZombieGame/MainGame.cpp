#include "MainGame.h"

#include <Bengine\Bengine.h>
#include <Bengine\InputManager.h>
#include <Bengine\Timing.h>
#include <Bengine\ErrorMess.h>
#include <random>
#include <ctime>
#include <algorithm>

#include <SDL\SDL.h>
#include <iostream>
#include "Zombie.h"
#include "Gun.h"


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f; 

MainGame::MainGame() :
	_screenWdith(1280),
	_screenHeight(720),
	_gameState(GameState::PLAY),
	_fps(0.0f),
	_player(nullptr),
	_numZombiesKilled(0)
{
}


MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}

void MainGame::run()
{
	initSystems();

	initLevel();

	Bengine::Music music = _audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);

	gameLoop();
}

void MainGame::initSystems()
{
	Bengine::init();
	//Intialized sound
	_audioEngine.init();

	_window.create("1st Game", _screenWdith, _screenHeight, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	initShaders();

	_agentSpriteBatch.init();
	_camera.init(_screenWdith, _screenHeight);
	
}

void MainGame::initLevel()
{
	//Level 1
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager,&_camera,&_bullets);

	_humans.push_back(_player);

     std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int>randX(2, _levels[_currentLevel]->getWidth()-2);
	std::uniform_int_distribution<int>randY(2, _levels[_currentLevel]->getHeight()- 2);

	//add all the random human
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
		_humans.push_back(new Human);
		glm::vec2 position(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, position);
	}

	//add the zombies
	const std::vector<glm::vec2>& zombiePosition = _levels[_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePosition.size(); i++) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED,zombiePosition[i]);
	}
	//set up the player guns
	const float BULLET_SPEED = 40.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 10.0f, 30, BULLET_SPEED,_audioEngine.loadSoundEffect("Sound/sortgun.wav")));
	_player->addGun(new Gun("Shotgun", 30, 20, 40.0f, 4, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/sortgun.wav")));
	_player->addGun(new Gun("MP5", 2, 1, 20.0f, 20, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/sortgun.wav")));
}

void MainGame::initShaders()
{
	_textureProgram.compileShaders("Shaders/colorShader_vert.txt", "Shaders/colorShader_frag.txt");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColour");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}


void MainGame::gameLoop()
{
	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;

	Bengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

      //  const float CAMERA_SCALE = 1.0f / 3.0f;
     //  _camera.setScale(CAMERA_SCALE);

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float previousTicks = SDL_GetTicks();
	
	while (_gameState == GameState::PLAY)
	{
		fpsLimiter.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();

		_inputManager.update();

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime =std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}

		_camera.setPosition(_player->getPosition());

		_camera.update();

		drawGame();

		_fps = fpsLimiter.end();             
		std::cout << _fps << std::endl;
	}
}

void MainGame::updateAgents(float deltaTime)
{  //update all humans
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies,
			deltaTime);
	}
	//update all zombie
	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies,
			deltaTime);
	}

	//update zombie collisions
	for (int i = 0; i <_zombies.size(); i++) {
		//collide with other zombie
		for (int j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgents(_zombies[j]);
		}
		//colide with human
		for (int j = 1; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgents(_humans[j])) {
				//add the new zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				//delete the zombie
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}
		//collide with player
		if (_zombies[i]->collideWithAgents(_player)) {
			Bengine::fatalError("YOU LOSE");
		}
	}

	//update human collisions
	for (int i = 0; i < _humans.size(); i++) {
		for (int j = i + 1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgents(_humans[j]); 
		}
	}
}

void MainGame::updateBullets(float deltaTime) {
	for (int i = 0; i < _bullets.size(); i++) {
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(),deltaTime)) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;

	//collide with human and Zombie
	for (int i = 0; i < _bullets.size(); i++) {
		wasBulletRemoved = false;
		for (int j = 0; j < _zombies.size(); ) {
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				//Damage zombies and kill it if its out of health
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else {
					j++;
				}

				//remove the bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--;
				break;
			}
			else {
				j++;
			}
		}

		//loop for humans
		if (wasBulletRemoved == false) {
			for (int j = 1; j < _humans.size();) {
				if (_bullets[i].collideWithAgent(_humans[j])) {
					//Damage  humans and kill it if its out of health
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
						delete  _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
					}
					else {
						j++;
					}

					//remove the bullet
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					_numHumansKilled++;
					i--;
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}

void MainGame::checkVictory() {
	if (_zombies.empty()) {
		std::printf("***You Win***\n You killed %d humans and %d zombies.There are %d/%d civilation remaining",
			_numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		Bengine::fatalError("");
	}
}


void MainGame::processInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releasedKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releasedKey(evnt.button.button);
			break;
		}
	}
}

void MainGame::drawGame()
{
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	_textureProgram.use();

	// IMPLEMENT THIS!
	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw the level
	_levels[_currentLevel]->draw();

	_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS *2.0f);

	//draw the humans
	for (int i = 0; i < _humans.size(); i++)
	{
		if (_camera.isBoxInView(_humans[i]->getPosition(), agentDims))
		{
			_humans[i]->draw(_agentSpriteBatch);
		}
	}
	//draw the zombie
	for (int i = 0; i < _zombies.size(); i++)
	{
		if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDims))
		{
			_zombies[i]->draw(_agentSpriteBatch);
		}
	}
	//draw the bullets
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();

	_agentSpriteBatch.renderBatch();

	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}
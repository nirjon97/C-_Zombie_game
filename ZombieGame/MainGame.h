#pragma once

#include <Bengine\Window.h>
#include <Bengine\GLSLProgram.h>
#include <Bengine\InputManager.h>
#include <Bengine\Camera2D.h>
#include <Bengine\SpriteBatch.h>
#include <Bengine\AudioEngine.h>
#include "Level.h"
#include "Player.h"
#include "Bullet.h"

class Zombie;

enum class GameState {
	PLAY,
	EXIT
};


class MainGame
{
public:
	MainGame();
	~MainGame();

	//Runs the game
	void run();

private:
	//init core systems
	void initSystems();

	//init the level AND SETS UP EVERYTHING oops caps
	void initLevel();

	//init shaders
	void initShaders();

	//Main game loop
	void gameLoop();

	//Updates all agents
	void updateAgents(float deltaTime);

	//Updates all bullets
	void updateBullets(float deltaTime);

	//Updates Victory condition
	void checkVictory();

	//input processing
	void processInput();

	//renders the game
	void drawGame();

	//Member variables
	Bengine::Window _window;
	Bengine::GLSLProgram _textureProgram;
	Bengine::InputManager _inputManager;
	Bengine::Camera2d _camera;
	Bengine::SpriteBatch _agentSpriteBatch;
	
	std::vector<Level*> _levels;

	int _screenWdith, _screenHeight;

	
	float _fps;
	int _currentLevel;

	Player* _player;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet>_bullets;

	int _numHumansKilled;
	int _numZombiesKilled;

	Bengine::AudioEngine _audioEngine;

	GameState _gameState;
};


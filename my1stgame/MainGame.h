#pragma once


#include <SDL\SDL.h>
#include <GL\glew.h>

#include <Bengine\Bengine.h>
#include <Bengine\Sprite.h>
#include <Bengine\GLTextures.h>
#include <Bengine\GLSLProgram.h>
#include <vector>
#include <Bengine\Window.h>
#include <Bengine\Camera2d.h>
#include <Bengine\SpriteBatch.h>
#include <Bengine\InputManager.h>
#include <Bengine\Timing.h>
#include "Bullets.h"

enum  GameState {PLAY,EXIT};
class MainGame
{
public:
	MainGame(void);
	~MainGame(void);

	void run();
	

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	
	

	Bengine::Window _window;
	int _screenWidth;
	int _screenHeight;

	//Time to send to graphic card-GLSL
	float _time;

	
	GameState _gameState;

	//Sprite _sprite;
	//Using pointer to allocate sprite on the heap-prevent to delete vertex buffer object when destroy the sprite
	//std::vector<Bengine::Sprite*> _sprite;

	Bengine::GLSLProgram _colorProgram;

	//Camera2d
	Bengine::Camera2d _camera; 

	//Sprite batch-using this to contain all the buffer data 
	//and draw it at once instead of draw it for everytime it get data from the CPU
	Bengine::SpriteBatch _spriteBatch;

	//GLTexture _playerTexture;

	//Input manager(mouse,keyboard...)
	Bengine::InputManager _inputManager;

	//FPS
	Bengine::FpsLimiter _fpsLimiter;
	float _fps;
	float _maxFPS;

	//Create bullets to shot
	std::vector<Bullets> _bullets;
	
};


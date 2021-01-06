#include "MainGame.h"
#include <string>
#include <iostream>
#include <Bengine/ErrorMess.h>
#include <glm\glm.hpp>
#include <Bengine\ResourceManage.h>


MainGame::MainGame(void):_screenWidth(1024),_screenHeight(560),_gameState(GameState::PLAY),_time(0),_maxFPS(120.0f)
{
	_camera.init(_screenWidth,_screenHeight);
}


MainGame::~MainGame(void)
{
}
void MainGame::run()	
{
	initSystems();

	//_sprite.push_back( new Bengine::Sprite());
	//_sprite.back()->init(-1.0f,-1.0f,1.0f,1.0f,"Textures/zero.png");
	//_sprite.push_back(new Bengine::Sprite());
	//_sprite.back()->init(0.0f,-1.0f,1.0f,1.0f,"Textures/zero.png");

	//Use actual camera coordinate
	/*_sprite.push_back( new Bengine::Sprite());
	_sprite.back()->init(0.0f,0.0f,_screenWidth/2,_screenWidth/2,"Textures/zero.png");
	_sprite.push_back(new Bengine::Sprite());
	_sprite.back()->init(_screenWidth/2,0.0f,_screenWidth/2,_screenWidth/2,"Textures/zero.png");


	_sprite.push_back(new Bengine::Sprite());
	_sprite.back()->init(0.0f,_screenWidth/2,_screenWidth/2,_screenWidth/2,"Textures/zero.png");
	_sprite.push_back(new Bengine::Sprite());
	_sprite.back()->init(_screenWidth/2,_screenWidth/2,_screenWidth/2,_screenWidth/2,"Textures/zero.png");*/

	gameLoop();
}

void MainGame::initSystems()
{

	//Init the engine
	Bengine::init();
	//Create window 
	_window.create("Game Engine",_screenWidth,_screenHeight,0);
	//Init the shader
	initShaders();

	//Init Sprite batch
	_spriteBatch.init();

	//Init FPS
	_fpsLimiter.init(_maxFPS);

}
void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShader_vert.txt","Shaders/colorShader_frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}
void MainGame::gameLoop()
{
	while(_gameState != GameState::EXIT){
		_fpsLimiter.begin();
		//Used for frame measuring-caculate the current frame time not average
		float startTicks = SDL_GetTicks();

		//Game init
		processInput();
		_time+=0.01;
		//Update camera before draw
		_camera.update();
		//Update all bullets before draw
		for(int i=0;i<_bullets.size();)
		{
			if(_bullets[i].update()){
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else{
				i++;
			}
		}
		drawGame();
		_fps = _fpsLimiter.end();
		//print only every 10 frames
		static int frameCounter =0;
		frameCounter ++;
		if(frameCounter==10000){
			std::cout<<_fps<<std::endl;
			frameCounter =0;
		}
	}
}
void MainGame::processInput()
{
	//Event of window game
	SDL_Event evnt;

	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.05f;
	while(SDL_PollEvent(&evnt))
	{
		switch (evnt.type){
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager. setMouseCoords(evnt.motion.x,evnt.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releasedKey(evnt.button.button);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releasedKey(evnt.key.keysym.sym);
			break;

		}
	}
	if(_inputManager.isKeyDown(SDLK_w))
			_camera.setPosition(_camera.getPosition()+glm::vec2(0.0f,-CAMERA_SPEED));
	if(_inputManager.isKeyDown(SDLK_s))
			_camera.setPosition(_camera.getPosition()+glm::vec2(0.0f,CAMERA_SPEED));	
	if(_inputManager.isKeyDown(SDLK_a))
			_camera.setPosition(_camera.getPosition()+glm::vec2(CAMERA_SPEED,0.0f));
	if(_inputManager.isKeyDown(SDLK_d))
			_camera.setPosition(_camera.getPosition()+glm::vec2(-CAMERA_SPEED,0.0f));
	if(_inputManager.isKeyDown(SDLK_q))
			_camera.setScale(_camera.getScale() + SCALE_SPEED);
	if(_inputManager.isKeyDown(SDLK_e))
		    _camera.setScale(_camera.getScale() - SCALE_SPEED);
	if(_inputManager.isKeyDown(SDL_BUTTON_LEFT)){
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreentoWorld(mouseCoords);
		std::cout << mouseCoords.x <<" "<<mouseCoords.y <<std::endl;
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition ;
		direction = glm::normalize(direction);

		//Bullets mybullets(playerPosition,direction,5.00f,1000);
		_bullets.emplace_back(Bullets(playerPosition,direction,5.00f,1000));
	}
		
			
}
void MainGame::drawGame()
{
	glClearDepth(1.0);
	//Fill background color support glClear Color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	/*Immediate mode-which send every vertex from CPU to GPU for drawing,too-slow don't use anymore
	//Enable color for triangle 
	glEnableClientState(GL_COLOR_ARRAY);
	glBegin(GL_TRIANGLES);
	//Draw color red
	glColor3f(1.0f,0.0f,0.0f);
	//Draw vertex
	glVertex2f(0,0);
	glVertex2f(0,250);
	glVertex2f(250,250);

	glEnd();*/
	//Use shader before draw sprite 
	_colorProgram.use();

	//Set the first texture to bind
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation,0);

	//Time to change color
	//GLuint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation,_time);

	//Set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	//Uniform call for camera matrix
	glUniformMatrix4fv(pLocation,1,GL_FALSE,&(cameraMatrix[0][0]));



	//Draw sprite using vertex object-it pack the vertex and send from CPU to GPU-not efficient, write just for now
	//for(int i=0; i<_sprite.size();i++)
	//{
		//_sprite[i]->draw();
	//}

	//draw with sprite batch
	_spriteBatch.begin();

	
	glm::vec4 pos(0.0f,0.0f,50.0f,50.0f);
	glm::vec4 uv(0.0f,0.0f,1.0f,1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManage::getTexture("Textures/zero.png");
	Bengine::ColorRGBA8 color;
	color.red = 255;
	color.green = 255;
	color.blue=255;
	color.alpha = 255;

	//draw megaman zero
	_spriteBatch.draw(pos,uv,texture.id,0.0f,color);
	//_spriteBatch.draw(pos+glm::vec4(50,0,0,0),uv,texture.id,0.0f,color);
	//_spriteBatch.draw(pos+glm::vec4(0,50,0,0),uv,texture.id,0.0f,color);
	//_spriteBatch.draw(pos+glm::vec4(50,50,0,0),uv,texture.id,0.0f,color);

	//draw bullets
	for(int i=0; i <_bullets.size();i++)
	{
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	//Unbind the texture after use
	glBindTexture(GL_TEXTURE_2D,0);
	//Free shader after use
	_colorProgram.unuse();
	//swap our buffer and draw everything on the screen
	_window.swapBuffer();


}


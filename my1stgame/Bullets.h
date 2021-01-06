#pragma once
#include <glm\glm.hpp>
#include <Bengine\SpriteBatch.h>
class Bullets
{
public:
	Bullets(glm::vec2 pos, glm::vec2 dir,float speed, int lifeTime);
	~Bullets(void);

	void draw(Bengine::SpriteBatch &spriteBatch);
	//return true if the bullet out of life 
	bool update();
private: 
	float _speed;
	int _lifeTime;
	glm::vec2 _direction;
	glm::vec2 _position;

};


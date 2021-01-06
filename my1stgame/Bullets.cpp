#include "Bullets.h"
#include <Bengine\ResourceManage.h>
#include <Bengine\GLTextures.h>


Bullets::Bullets(glm::vec2 pos, glm::vec2 dir,float speed, int lifeTime):
_lifeTime(lifeTime),_position(pos),_direction(dir),_speed(speed)
{
}


Bullets::~Bullets(void)
{
}

void Bullets::draw(Bengine::SpriteBatch &spriteBatch)
{
	glm::vec4 uv(0.0f,0.0f,1.0f,1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManage::getTexture("Textures/bullets.png");
	Bengine::ColorRGBA8 color;
	color.red = 255;
	color.green = 255;
	color.blue=255;
	color.alpha = 255;
	glm::vec4 posAndSize = glm::vec4(_position.x,_position.y,30,30);
	
	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullets::update()
{
	_position += _direction * _speed;
	_lifeTime--;
	if(_lifeTime ==0)
		return true;
	return false;
}
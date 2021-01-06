#pragma once

#include <glm\glm.hpp>
#include <Bengine\SpriteBatch.h>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData,
		                std::vector<Human*>&humans,
						std::vector<Zombie*>&zombies, float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgents(Agent* agent);

	void draw(Bengine::SpriteBatch& _spriteBatch);

	//return true if we died
	bool applyDamage(float damage);

	glm::vec2 getPosition() const { return _position; }

protected:
	void checkTilePosition(const std::vector<std::string>& levelData,
		std::vector<glm::vec2>& collideTilePosition,
		float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 _position;
	float _speed;
	Bengine::ColorRGBA8 _color;
	float _health;
	GLuint _textureID;
};


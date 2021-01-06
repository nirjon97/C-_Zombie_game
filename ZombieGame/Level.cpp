#include "Level.h"

#include <Bengine\ErrorMess.h>
#include <iostream>
#include <fstream>
#include <Bengine\ResourceManage.h>

Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (file.fail())
	{
		Bengine::fatalError("Failed to open " + fileName);
	}

	//Throw away first string in temp;
	std::string temp;
	file >> temp >> _numHumans;

	std::getline(file, temp);//throw away the rest of the first line

	//read the level data
	while (std::getline(file, temp))
	{
		_levelData.push_back(temp);
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::ColorRGBA8 whiteColour;
	whiteColour.red = 255;
	whiteColour.green = 255;
	whiteColour.blue = 255;
	whiteColour.alpha = 255;

	std::reverse(_levelData.begin(), _levelData.end());///< Flip the y axis

	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			char tile = _levelData[y][x];

			//set dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) {
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManage::getTexture("Textures/baum.png").id,
					0.0f,
					whiteColour);
				break;
			case 'G':
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManage::getTexture("Textures/s3tree3_02.png").id,
					0.0f,
					whiteColour);
				break;
			case 'L':
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManage::getTexture("Textures/tree5_00.png").id,
					0.0f,
					whiteColour);
				break;
			case '@':
				_levelData[y][x] = '.';
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
			}
		}
	}


	_spriteBatch.end();
}

Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}

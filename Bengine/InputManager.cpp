#include "InputManager.h"

namespace Bengine{
	InputManager::InputManager(void):_mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager(void)
	{
	}

	void InputManager::update(){
		//loop through _keymap using a each loop,copy it over to _previouskeymap
		for (auto& it : _keyMap) {
			_previouskeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}
	 
	void InputManager::releasedKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	void  InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y ;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()){
			return it->second;
	}
else {
	return false;
}
	}


bool InputManager::isKeyPressed(unsigned int keyID) {
	if (isKeyDown(keyID) == true && wasKeyPressed(keyID) == false) {
		return true;
	}
	return false;
}

bool InputManager::wasKeyPressed(unsigned int keyID)
{
	auto it = _previouskeyMap.find(keyID);
	if (it != _previouskeyMap.end()) {
		return it->second;
	}
	else {
		return false;
	}
}
}
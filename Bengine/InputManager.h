#pragma once

#include <unordered_map>
#include <glm\glm.hpp>

namespace Bengine{
	class InputManager
	{
	public:
		InputManager(void);
		~InputManager(void);

		void update();

		void pressKey(unsigned int keyID);
		void releasedKey(unsigned int keyID);
		//return true if the key held down keydown
		bool isKeyDown(unsigned int keyID);
		//return true if the key just pressed 
		bool isKeyPressed(unsigned int keyID);

		void setMouseCoords(float x, float y);

		//getter
		glm::vec2 getMouseCoords() const{return _mouseCoords;}
	private:
		//return true if the key held down keydown
		bool wasKeyPressed(unsigned int keyID);
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previouskeyMap;
		glm::vec2 _mouseCoords;
	};
}

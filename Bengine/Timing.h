#pragma once

namespace Bengine{
	class FpsLimiter{
	public:
		FpsLimiter();

		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();
		
		//End return the current FPS
		float end();
	private:
		void calculateFPS();
		//FPS calculate
		float _fps;
		float _frameTime;
		float _maxFPS;
		unsigned int _startTicks;

	};
}
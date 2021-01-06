#include "ErrorMess.h"
#include <iostream>
#include <cstdlib>
#include <SDL\SDL.h>
namespace Bengine{
	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout<<"Enter any key to exit..."<<std::endl;
		int in;
		std::cin>>in;
		SDL_Quit();
		exit(1);
	}
}

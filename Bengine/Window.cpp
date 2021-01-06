#include "Window.h"
#include "ErrorMess.h"

namespace Bengine{
	Window::Window(void)
	{
	}


	Window::~Window(void)
	{
	}
	int Window::create(std::string windowName,int screenWidth,int screenHeight,unsigned int currentFlags )
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if(currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if(currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if(currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}
		//Open a SDL window
		_sdlwindow =SDL_CreateWindow(windowName.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screenWidth,screenHeight,SDL_WINDOW_OPENGL);

		if(_sdlwindow == nullptr)
		{
			fatalError("SDL window could not be created!");

		}
		SDL_GLContext glContext  = SDL_GL_CreateContext(_sdlwindow);
		if(glContext == nullptr)
		{
			fatalError("SDL_GL context could not be created!");
		}
		GLenum error =  glewInit();
		if(error != GLEW_OK)
		{
			fatalError("Could not initialize glew!");
		}

		//CHECK THE OPEN GL VERSION
		//std::printf("OPENGL version:%s",glGetString(GL_VERSION));
		//Fill background window with blue color
		glClearColor(0.0f,0.0f,1.0f,1.0f);

		//Enable VSYNC(Verticle synchronize)-to avoid the scene tearing-it synchonize the frame rate with the frame of monitor,it wait monior refresh to draw frame
		//SDL_GL_SetSwapInterval(1);//SET 1 TO ENABLE, 0 TO DISABLE

		//Enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}
	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_sdlwindow);
	}
}
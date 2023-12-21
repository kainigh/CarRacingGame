#include "Window.h"
#include "Log.h"
#include <iostream>


bool Window::initialize()
{

	

	int sdlInitResult = SDL_Init(SDL_INIT_EVERYTHING);
	if (sdlInitResult != 0) {

		Log::error(LogCategory::Video, "Unable to initialize SDL");
			return false;
	}

	SDLWindow = SDL_CreateWindow("Car Racing", 100, 100, width, height, 0); // O is flag we will use later


	if (!SDLWindow)
	{
	Log::error(LogCategory::System, "Failed to create window");
		return false;
	}
	return true;

}

void Window::close()
{
	SDL_DestroyWindow(SDLWindow);
}

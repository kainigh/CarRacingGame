#pragma once
#include <SDL.h>


constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;


class Window
{

public:
	Window() = default;
	Window(const Window&) = delete;

	bool initialize();
	void close();

	SDL_Window* getSDLWindow() const { return SDLWindow; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

	

private:

	SDL_Window* SDLWindow{ nullptr };
	int width{ WINDOW_WIDTH };
	int height{ WINDOW_HEIGHT };



};


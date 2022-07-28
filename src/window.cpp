#include "window.h"

Window::Window(int sWidth, int sHeight, bool sFullScreen, bool sVsync) {
	window = NULL;
	this->sWidth = sWidth;
	this->sHeight = sHeight;
	this->sFullScreen = sFullScreen;
	this->sVsync = sVsync;
}

Window::~Window() {

}

bool Window::init() {
	window = SDL_CreateWindow("Argentum Online C++",
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		sWidth, sHeight, 
		SDL_WINDOW_SHOWN);

	if (sFullScreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return window != NULL;
}

void Window::closeWindow() {
	if(window != NULL) SDL_DestroyWindow(window);
}

SDL_Renderer* Window::createRenderer() {
	if (sVsync) 
	{
		return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	else 
	{
		return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}
}

void Window::setVsync(bool bVSYNC) 
{
	sVsync = bVSYNC;
}

void Window::changeScreen(bool bFullScreen) 
{
	sFullScreen = bFullScreen;

	if (bFullScreen) 
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	} 
	else 
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);
	}
}
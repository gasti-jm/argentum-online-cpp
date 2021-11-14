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
	const char * form_caption = "Argentum Online";

	if (sFullScreen) {
		window = SDL_CreateWindow(form_caption,
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED,
			sWidth, sHeight, 
			SDL_WINDOW_FULLSCREEN);
	} else {
		// Create window
		window = SDL_CreateWindow(form_caption,
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED,
			sWidth, sHeight,
			SDL_WINDOW_SHOWN);
	}

	return window != NULL;
}

void Window::closeWindow() {
	if(window != NULL) SDL_DestroyWindow(window);
}

SDL_Renderer* Window::createRenderer() {
	if (sVsync) {
		return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	} else {
		return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}
}

void Window::setVsync(bool v) {
	sVsync = v;
}

void Window::changeScreen(bool fullscreen) {
	sFullScreen = fullscreen;

	if (fullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	} else {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);
	}
}
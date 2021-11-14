#pragma once

#include <SDL.h>
#include <string>

class Window {
	private:
		SDL_Window* window;
		int sWidth, sHeight;
		bool sFullScreen, sVsync;

	public:
		Window(int, int, bool, bool);
		~Window();

		bool init();
		void closeWindow();

		void changeScreen(bool);
		void setVsync(bool);

		SDL_Renderer* createRenderer();
};
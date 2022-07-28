#pragma once
#include <sstream>
#include "texture.h"

class GUI {
	private:
		std::stringstream pathFile;
		SDL_Renderer* renderer; // same.
		Texture tGUI;

	public:
		GUI(SDL_Renderer*&, std::string);
		~GUI();

		void renderGUI(int, int);
		void setNewRenderer(SDL_Renderer*&);
};
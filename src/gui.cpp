#include "gui.h"

GUI::GUI(SDL_Renderer*& renderer, std::string file) 
{
	pathFile << "Resources/gui/" << file;
	this->renderer = renderer;
	tGUI.loadFromFile(renderer, pathFile.str().c_str());
}

GUI::~GUI() 
{
	tGUI.freeMemory();
	SDL_DestroyRenderer(renderer);
}

void GUI::renderGUI(int x, int y) 
{
	tGUI.render(renderer, x, y);
}

void GUI::setNewRenderer(SDL_Renderer*& renderer) 
{
	this->renderer = renderer;
	tGUI.freeMemory();
	tGUI.loadFromFile(renderer, pathFile.str().c_str());
}
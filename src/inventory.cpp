#include "inventory.h"

Inventory::Inventory(SDL_Renderer*& renderer, int maxSlots) 
{
	this->maxSlots = maxSlots;

	ihInterface = new GUI(renderer, "inventory.png");
	tSlot = new GUI(renderer, "slot.png");
}

Inventory::~Inventory() 
{

}

void Inventory::drawInterface(int x, int y) 
{
	ihInterface->renderGUI(x, y);
}

void Inventory::drawSlot(int x, int y) 
{
	tSlot->renderGUI(x, y);
}

void Inventory::setNewRenderer(SDL_Renderer*& renderer) 
{
	ihInterface->setNewRenderer(renderer);
	tSlot->setNewRenderer(renderer);
}

int Inventory::getMaxSlots() 
{
	return maxSlots;
}

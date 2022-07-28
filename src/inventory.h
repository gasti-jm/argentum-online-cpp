#pragma once

#include <SDL.h>
#include "gui.h"

class Inventory {
	private:
		int maxSlots;
		GUI* ihInterface;
		GUI* tSlot;

	public:
		Inventory(SDL_Renderer*&, int);
		~Inventory();

		void drawInterface(int, int);
		void drawSlot(int, int);

		void setNewRenderer(SDL_Renderer*&);

		int getMaxSlots();
};
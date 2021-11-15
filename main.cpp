#include "engine.h"

int main(int argc, char* args[]){
	Engine* engine = new Engine();

	if (engine->init()) {
		while (engine->getPrgRun()) {
			engine->showNextFrame();
			engine->checkInput();
			engine->checkKeys();
		}
	}

	engine->close();
	delete engine;

	return 0;
}

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "window.h"
#include "audio.h"
#include "resources.h"
#include "surface.h"
#include "chars.h"

class Engine {
	private:
		Window* eWindow;
		SDL_Renderer* renderer;
		Audio audio;
		SDL_Event e;

		Resources resources;
		Surface surface;

		Char charlist[10000];
		addToUser AddToUserPos;
		position User;
		bool UserMoving;

		bool prgRun; // main loop
		bool inChangeRenderer;
		// Timers and FPS
		bool showFPS;
		Uint32 FPS;
		Uint32 lFrameTimer;
		int countedFrames;
		Uint32 end_time;
		Uint32 timerElapsedTime;
		float timerTicksPerFrame;
		const float engineBaseSpeed = 0.02;
		const int TILE_PIXEL_SIZE = 32;
		const int SCROLL_PIXELS_PER_FRAME = 8;

	public:
		Engine();
		~Engine();

		bool init();
		void close();

		int sgn(short);

		void showNextFrame();
		void renderScreen(int, int, int, int);
		void charRender(int, int, int);
		void checkInput();
		void checkKeys();
		void updateFramesPerSeconds();

		void drawTextureGrhIndex(int, int, int, Uint8, Uint8, Uint8, double = 0.0, bool = false, Uint8 = 255);
		void drawTexture(grh&, int, int, bool = false, bool = false, bool = false, Uint8 = 255, double = 0.0);
		void drawText(std::string, int, int, Uint8 = 255, Uint8 = 255, Uint8 = 255, int = 0);
		void drawQuad(int, int, int, int, Uint8, Uint8, Uint8, Uint8, bool = false);
		void drawLine(int, int, int, int, Uint8, Uint8, Uint8, Uint8);
		int text_Width(std::string);

		void makeChar(int, eHeading, int, int);
		void moveScreen(eHeading);
		void moveCharByHead(int, eHeading);
		bool inMapBounds(int, int);
		bool moveToLegalPos(int, int);
		void moveTo(eHeading);
		void setCharacterFx(int, int, int);
		bool hayAgua(int, int);
		void doPasosFX(int);

		Uint32 getElapsedTime();
		bool getPrgRun();
};
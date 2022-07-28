#pragma once

#include <SDL_mixer.h>
#include <string>
#include <sstream>

#include "errors.h"

class Audio {
	private:
		Mix_Music* music = NULL;
		Mix_Chunk* sound = NULL;

	public:
		Audio();
		~Audio();

		bool init();
		void close();

		void playMusic(std::string, bool = true);
		void stopMusic();
		void unPause_Music();

		void playSound(std::string);

		void setMusicVolume(int);
		void setSoundsVolume(int);
};
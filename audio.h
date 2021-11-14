#pragma once

#include <SDL_mixer.h>
#include <string>
#include <sstream>

class Audio {
	private:
		//const std::string SND_PASOS1 = "23";
		//const std::string SND_PASOS2 = "24";

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
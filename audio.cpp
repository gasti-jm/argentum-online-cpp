#include "audio.h"

Audio::Audio() {

}

Audio::~Audio() {

}

bool Audio::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf ("No se pudo inicializar el sistema de audio! SDL ERROR: %s \n", Mix_GetError());
        return false;
    }

	return true;
}

void Audio::close() {
    if (sound != NULL) {
        Mix_FreeChunk(sound);
        sound = NULL;
    }

    if (music != NULL) {
        Mix_FreeMusic(music);
        music = NULL;
    }

    delete sound;
    delete music;
}

void Audio::playMusic(std::string nameMusic, bool isMidi) {
    std::stringstream path;

    if (isMidi) {
        path << "Resources/music/" << nameMusic << ".mid";
    } else {
        path << "Resources/music/" << nameMusic << ".mp3"; // por la intro de AO.
    }

    // ¿Hay musica reproduciendose?
    if (music != NULL) {
        stopMusic();
    }

    //Load music
    music = Mix_LoadMUS(path.str().c_str());
    if (music == NULL) {
        printf("Error al cargar la musica! SDL_Mixer ERROR: %s \n", Mix_GetError());
    } else {
        Mix_PlayMusic(music, -1); // Reproducimos.
    }
}

void Audio::stopMusic() {
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    music = NULL;
}

void Audio::unPause_Music() {
    if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic(); // Resume the music
    } else {
        Mix_PauseMusic(); // Pause the music
    }
}

void Audio::setMusicVolume(int volume) {
    Mix_VolumeMusic(MIX_MAX_VOLUME - volume);
}

void Audio::setSoundsVolume(int volume) {
    Mix_Volume(-1, MIX_MAX_VOLUME - volume);
}

void Audio::playSound(std::string nameSound) {
    std::stringstream path;
    path << "Resources/sounds/" << nameSound << ".wav";

    if (sound != NULL) {
        Mix_FreeChunk(sound);
        sound = NULL;
    }

    //Load sound effects
    sound = Mix_LoadWAV(path.str().c_str());
    if (sound == NULL) {
        printf("Error al cargar los efectos de sonido! SDL_mixer Error: %s \n", Mix_GetError());
    } else {
        Mix_PlayChannel(-1, sound, 0); // Suena una sola vez.
    }
}
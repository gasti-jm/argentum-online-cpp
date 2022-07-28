#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <sstream>

#include "errors.h"

class Texture {
    private:
        SDL_Texture* Textura;
        int tWidth, tHeight;

    public:
        Texture();
        ~Texture();

        bool loadFromFile(SDL_Renderer*, std::string);
        void freeMemory();
        void setColor(Uint8, Uint8, Uint8);
        void setBlend(SDL_BlendMode);
        void setAlpha(Uint8);
        void render(SDL_Renderer*, int, int, SDL_Rect* = NULL, double = 0.0, SDL_Point* = NULL);

        int getWidth();
        int getHeight();
};
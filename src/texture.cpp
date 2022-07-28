#include "texture.h"

Texture::Texture() {
    Textura = NULL;
    tWidth = 0;
    tHeight = 0;
}

Texture::~Texture() {
    freeMemory();
}

bool Texture::loadFromFile(SDL_Renderer* newRenderer, std::string path) {
    freeMemory();

    SDL_Texture* newTexture = NULL;

    // Cargamos la imagen en cierta ruta.
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::stringstream msgError;
        msgError << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError();
        Error::logError(msgError.str().c_str());
    } else {
        // Quitamos el fondo negro de la imagen cargada!
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));
        newTexture = SDL_CreateTextureFromSurface(newRenderer, loadedSurface);

        if (newTexture == NULL) {
            std::stringstream msgError;
            msgError << "Unable to create texture from " << path.c_str() <<"! SDL Error: " << SDL_GetError();
            Error::logError(msgError.str().c_str());
        } else {
            tWidth = loadedSurface->w;
            tHeight = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface); // liberamos memoria de la imagen cargada anteriormente antes de editar.
    }

    Textura = newTexture;
    return Textura != NULL;
}

void Texture::freeMemory() {
    if (Textura != NULL) { // Destruir en caso de existir.
        SDL_DestroyTexture(Textura);
        Textura = NULL;
        tWidth = 0;
        tHeight = 0;
    }
}

void Texture::setColor(Uint8 R, Uint8 G, Uint8 B) {
    SDL_SetTextureColorMod(Textura, R, G, B);
}

void Texture::setBlend(SDL_BlendMode blend) {
    SDL_SetTextureBlendMode(Textura, blend);
}

void Texture::setAlpha(Uint8 A) {
    SDL_SetTextureAlphaMod(Textura, A);
}

void Texture::render(SDL_Renderer* newRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center) {
    SDL_Rect RECT = { x, y, tWidth, tHeight };

    if (clip != NULL) {
        RECT.w = clip->w;
        RECT.h = clip->h;
    }

    SDL_RenderCopyEx(newRenderer, Textura, clip, &RECT, angle, center, SDL_FLIP_NONE);
}

int Texture::getWidth() {
    return tWidth;
}

int Texture::getHeight() {
    return tHeight;
}
#pragma once

#include "texture.h"

struct nodoTextures {
	int fileNumLoaded;
	Texture textureLoaded;
	nodoTextures* siguiente;
};

class Surface {
	private:
		nodoTextures* listTLoaded = NULL; // Lista
		nodoTextures* Textura = new nodoTextures();; // toma un nodo de la lista y lo renderiza.

	public:
		Surface();
		~Surface();

		void ADD_NEW_TEXTURE(SDL_Renderer*&, nodoTextures*&, int);
		void SEARCH_TEXTURE(nodoTextures*, int);
		void checkTextureExist(SDL_Renderer*&, int);
		void DELETE_ALL_TEXTURES();

		nodoTextures*& gTextura();
};
#include "engine.h"

Engine::Engine() {
	eWindow = NULL;
	renderer = NULL;
	prgRun = true;
	showFPS = false;

	FPS = 0;
	lFrameTimer = 0;
	countedFrames = 0;
	end_time = 0;
	timerElapsedTime = 0;
	timerTicksPerFrame = 0;
    UserMoving = false;
    inChangeRenderer = false;
}

Engine::~Engine() {

}

bool Engine::getPrgRun() {
    return prgRun;
}

bool Engine::init() {
	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
		return false;
	} 

    if (!audio.init()) return false;

    // Load Resources
    if (!resources.loadResources()) {
        return false;
    }

	// Create window
	eWindow = new Window(resources.getCFG().screen_width,
		resources.getCFG().screen_height,
		resources.getCFG().fullscreen,
		resources.getCFG().vsync);
	
	if (!eWindow->init()) {
		printf("Window could not be created! SDL_Error: %s \n", SDL_GetError());
		return false;
	}

	// Create Render
	renderer = eWindow->createRenderer();
	if (renderer == NULL) {
		printf("Render could not be created! SDL_Error: %s \n", SDL_GetError());
		return false;
	}

	//PNG SYSTEM LOADING
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_Image could not initialize! SDL_image Error: %s \n", IMG_GetError());
		return false;
	}

    audio.playMusic("4");
    //audio.playMusic("101", false);
    makeChar(0, eHeading::SOUTH, 50, 50);
    User.x = User.y = 50;

	// Init OK!
	return true;
}

void Engine::close() {
	if (renderer != NULL) SDL_DestroyRenderer(renderer);
    
    if (eWindow != NULL) {
        eWindow->closeWindow();
        delete eWindow;
    }

    //delete[] charlist;

    audio.close();

    Mix_Quit();
    IMG_Quit();
	SDL_Quit();
}

int Engine::sgn(short number) {
    if (number == 0) return 0;
    return (number / abs(number));
}

void Engine::showNextFrame() {
    static float OffsetCounterX, OffsetCounterY;

    if (inChangeRenderer) {
        SDL_DestroyRenderer(renderer);
        renderer = eWindow->createRenderer();

        // las texturas se almacenan en un render, y como este mismo muere tengo que borrar todas y recargarlas.
        surface.DELETE_ALL_TEXTURES();

        inChangeRenderer = false; // finalizamos el proceso.
        return; // y no dibujes nada.
    }

    if (UserMoving){
        if (AddToUserPos.X != 0){
            OffsetCounterX -= SCROLL_PIXELS_PER_FRAME * AddToUserPos.X * timerTicksPerFrame;
            if (abs(OffsetCounterX) >= abs(TILE_PIXEL_SIZE * AddToUserPos.X)){
                OffsetCounterX = AddToUserPos.X = 0;
                UserMoving = false;
            }
        }

        if (AddToUserPos.Y != 0){
            OffsetCounterY -= SCROLL_PIXELS_PER_FRAME * AddToUserPos.Y * timerTicksPerFrame;
            if (abs(OffsetCounterY) >= abs(TILE_PIXEL_SIZE * AddToUserPos.Y)){
                OffsetCounterY = AddToUserPos.Y = 0;
                UserMoving = false;
            }
        }
    }

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer); // Clear screen

	renderScreen(User.x - AddToUserPos.X,
                 User.y - AddToUserPos.Y,
                 OffsetCounterX, OffsetCounterY);

	updateFramesPerSeconds();
	SDL_RenderPresent(renderer); // Update screen
}

void Engine::renderScreen(int tilex, int tiley, int PixelOffsetX, int PixelOffsetY) {
    int y; int x;
    int screenminY; int screenmaxY;
    int screenminX; int screenmaxX;
    int minY; int maxY;
    int minX; int maxX;
    int ScreenX = 0; int ScreenY = 0;
    int minXOffset = 0; int minYOffset = 0;

    screenminY = tiley - ((resources.getCFG().screen_height / TILE_PIXEL_SIZE) / 2);
    screenmaxY = tiley + ((resources.getCFG().screen_height / TILE_PIXEL_SIZE) / 2);
    screenminX = tilex - ((resources.getCFG().screen_width / TILE_PIXEL_SIZE) / 2);
    screenmaxX = tilex + ((resources.getCFG().screen_width / TILE_PIXEL_SIZE) / 2);
    
    minY = screenminY - 1;
    maxY = screenmaxY + 1;
    minX = screenminX - 1;
    maxX = screenmaxX + 1;

    if (minY < 0){
        minYOffset = 0 - minY;
        minY = 0;
    }

    if (maxY >= 99) maxY = 99;

    if (minX < 0){
        minXOffset = 0 - minX;
        minX = 0;
    }

    if (maxX >= 99) maxX = 99;

    if (screenminY > 0) {
        screenminY--;
    } else {
        screenminY = 0;
        ScreenY = 0;
    }

    if (screenmaxY < 99) screenmaxY = screenmaxY + 1;

    if (screenminX > 0){
        screenminX--;
    } else {
        screenminX = 0;
        ScreenX = 0;
    }

    if (screenmaxX < 99) screenmaxX = screenmaxX + 1;

    ScreenY = minYOffset - 1;
    for (y = minY; y <= maxY; y++) {
        ScreenX = minXOffset - 1;
        for (x = minX; x <= maxX; x++) {

            if (resources.getMapData(x, y).layer[0].grhIndex != 0) {
                drawTexture(resources.getMapData(x, y).layer[0],
                    ScreenX * TILE_PIXEL_SIZE + PixelOffsetX,
                    ScreenY * TILE_PIXEL_SIZE + PixelOffsetY, true, true);
            }

            if (resources.getMapData(x, y).layer[1].grhIndex != 0) {
                drawTexture(resources.getMapData(x, y).layer[1],
                    ScreenX * TILE_PIXEL_SIZE + PixelOffsetX,
                    ScreenY * TILE_PIXEL_SIZE + PixelOffsetY, true, true);
            }

            ScreenX++;
        }
        ScreenY++;
    }

    ScreenY = minYOffset - 1;
    for (y = minY; y <= maxY; y++){
        ScreenX = minXOffset - 1;
        for (x = minX; x <= maxX; x++){

            if (resources.getMapData(x, y).objgrh.grhIndex != 0) {
                drawTexture(resources.getMapData(x, y).objgrh,
                    ScreenX * TILE_PIXEL_SIZE + PixelOffsetX,
                    ScreenY * TILE_PIXEL_SIZE + PixelOffsetY, true, true);
            }

            if (resources.getMapData(x, y).charindex > -1){
                charRender(resources.getMapData(x, y).charindex, 
                    (ScreenX * TILE_PIXEL_SIZE + PixelOffsetX), (ScreenY * TILE_PIXEL_SIZE + PixelOffsetY));
            }

            if (resources.getMapData(x, y).layer[2].grhIndex != 0) {
                drawTexture(resources.getMapData(x, y).layer[2],
                    ScreenX * TILE_PIXEL_SIZE + PixelOffsetX,
                    ScreenY * TILE_PIXEL_SIZE + PixelOffsetY, true, true);
            }

            ScreenX++;
        }
        ScreenY++;
    }

    ScreenY = minYOffset - 1;
    for (y = minY; y <= maxY; y++){
        ScreenX = minXOffset - 1;
        for (x = minX; x <= maxX; x++){
            if (resources.getMapData(x, y).layer[3].grhIndex != 0) {
                drawTexture(resources.getMapData(x, y).layer[3],
                    ScreenX * TILE_PIXEL_SIZE + PixelOffsetX,
                    ScreenY * TILE_PIXEL_SIZE + PixelOffsetY, true, true);
            }
            ScreenX++;
        }
        ScreenY++;
    }
  
    if (showFPS) {
        std::stringstream FPSText;
        FPSText << "FPS: " << FPS;
        drawText(FPSText.str().c_str(), 0, 0);
    }
}

void Engine::charRender(int charIndex, int pixelOffSetX, int pixelOffSetY) {
    Uint8 charAlpha = 255;
    color nickColor;
    bool moved = false;

    if (!charlist[charIndex].moving) {
        if (charlist[charIndex].scrollDirectionX != 0) {
            charlist[charIndex].moveOffSetX += SCROLL_PIXELS_PER_FRAME * sgn(charlist[charIndex].scrollDirectionX) * timerTicksPerFrame;

            if (charlist[charIndex].Body.walk[charlist[charIndex].Heading].speed > 0.0f) {
                charlist[charIndex].Body.walk[charlist[charIndex].Heading].started = true;
            }

            charlist[charIndex].Weapon.walk[charlist[charIndex].Heading].started = true;
            charlist[charIndex].Shield.walk[charlist[charIndex].Heading].started = true;

            moved = true;

            if ((sgn(charlist[charIndex].scrollDirectionX) == 1 && charlist[charIndex].moveOffSetX >= 0)
                || (sgn(charlist[charIndex].scrollDirectionX) == -1 && charlist[charIndex].moveOffSetX <= 0)) {
                charlist[charIndex].moveOffSetX = 0;
                charlist[charIndex].scrollDirectionX = 0;
            }
        }

        if (charlist[charIndex].scrollDirectionY != 0) {
            charlist[charIndex].moveOffSetY += SCROLL_PIXELS_PER_FRAME * sgn(charlist[charIndex].scrollDirectionY) * timerTicksPerFrame;

            if (charlist[charIndex].Body.walk[charlist[charIndex].Heading].speed > 0.0f) {
                charlist[charIndex].Body.walk[charlist[charIndex].Heading].started = true;
            }
            charlist[charIndex].Weapon.walk[charlist[charIndex].Heading].started = true;
            charlist[charIndex].Shield.walk[charlist[charIndex].Heading].started = true;

            moved = true;

            if ((sgn(charlist[charIndex].scrollDirectionY) == 1 && charlist[charIndex].moveOffSetY >= 0)
                || (sgn(charlist[charIndex].scrollDirectionY) == -1 && charlist[charIndex].moveOffSetY <= 0)) {
                charlist[charIndex].moveOffSetY = 0;
                charlist[charIndex].scrollDirectionY = 0;
            }

        }
    }

    if (!moved){
        charlist[charIndex].Body.walk[charlist[charIndex].Heading].started = false;
        charlist[charIndex].Body.walk[charlist[charIndex].Heading].frameCounter = 0;

        charlist[charIndex].Weapon.walk[charlist[charIndex].Heading].started = false;
        charlist[charIndex].Weapon.walk[charlist[charIndex].Heading].frameCounter = 0;

        charlist[charIndex].Shield.walk[charlist[charIndex].Heading].started = false;
        charlist[charIndex].Shield.walk[charlist[charIndex].Heading].frameCounter = 0;

        charlist[charIndex].moving = false;
    }

    pixelOffSetX += (int)charlist[charIndex].moveOffSetX;
    pixelOffSetY += (int)charlist[charIndex].moveOffSetY;

    if (charlist[charIndex].invisible) charAlpha = 128;

    // body
    if (charlist[charIndex].Body.walk[charlist[charIndex].Heading].grhIndex > 0) {
        drawTexture(charlist[charIndex].Body.walk[charlist[charIndex].Heading],
                    pixelOffSetX, pixelOffSetY, true, true, false, charAlpha);
    }

    // head
    if (charlist[charIndex].Head.walk[charlist[charIndex].Heading].grhIndex != 0) {
        drawTexture(charlist[charIndex].Head.walk[charlist[charIndex].Heading],
                    pixelOffSetX + charlist[charIndex].Body.headOffsetX,
                    pixelOffSetY + charlist[charIndex].Body.headOffsetY, true, true, false, charAlpha);
    }

    // helmet
    if (charlist[charIndex].Helmet.walk[charlist[charIndex].Heading].grhIndex != 0) {
        drawTexture(charlist[charIndex].Helmet.walk[charlist[charIndex].Heading],
                    pixelOffSetX + charlist[charIndex].Body.headOffsetX,
                    pixelOffSetY + charlist[charIndex].Body.headOffsetY - 34, true, true, false, charAlpha);
    }

    // weapon
    if (charlist[charIndex].Weapon.walk[charlist[charIndex].Heading].grhIndex != 0) {
       drawTexture(charlist[charIndex].Weapon.walk[charlist[charIndex].Heading],
                   pixelOffSetX, pixelOffSetY, true, true, false, charAlpha);
    }

    // shield
    if (charlist[charIndex].Shield.walk[charlist[charIndex].Heading].grhIndex != 0) {
        drawTexture(charlist[charIndex].Shield.walk[charlist[charIndex].Heading],
                    pixelOffSetX, pixelOffSetY, true, true, false, charAlpha);
    }

    switch (charlist[charIndex].priv) {
        case 0: nickColor.r = 128; nickColor.g = 0; nickColor.b = 0; break;
        case 1: nickColor.r = 0; nickColor.g = 128; nickColor.b = 255; break;
        case 2: nickColor.r = 0; nickColor.g = 128; nickColor.b = 0; break;
    }

    if (charlist[charIndex].name != "") {
        drawText(charlist[charIndex].name,
                 pixelOffSetX + 16 - text_Width(charlist[charIndex].name) / 2, pixelOffSetY + 30,
                 nickColor.r, nickColor.g, nickColor.b);
    }

    if (charlist[charIndex].clan != "") {
        drawText(charlist[charIndex].clan,
                 pixelOffSetX + 16 - text_Width(charlist[charIndex].clan) / 2, pixelOffSetY + 45,
                 nickColor.r, nickColor.g, nickColor.b);
    }

    /** Draw FX */
    if (charlist[charIndex].fxIndex != -1) {
        drawTexture(charlist[charIndex].fX,
                    pixelOffSetX + resources.getFX(charlist[charIndex].fxIndex).OffsetX,
                    pixelOffSetY + resources.getFX(charlist[charIndex].fxIndex).OffsetY,
                    true, true, true);

        /** Check if animation is over */
        if (charlist[charIndex].fX.started == false) charlist[charIndex].fxIndex = -1;
    }
}

void Engine::setCharacterFx(int charIndex, int fx, int loops) {
    charlist[charIndex].fxIndex = fx;

    if (charlist[charIndex].fxIndex > -1) {
        resources.initGrh(charlist[charIndex].fX, resources.getFX(fx).Animacion, true);
        charlist[charIndex].fX.loops = loops;
    }
}

void Engine::updateFramesPerSeconds() {
	if (SDL_GetTicks() >= lFrameTimer + 1000) { // Actualizamos al pasar 1 seg.
		lFrameTimer = SDL_GetTicks();
		FPS = countedFrames;
		countedFrames = 0;
	}

	countedFrames++;
	timerElapsedTime = getElapsedTime();
	timerTicksPerFrame = (timerElapsedTime * engineBaseSpeed);
}

Uint32 Engine::getElapsedTime() {
	Uint32 start_time = SDL_GetTicks();
	Uint32 ms = (start_time - end_time);
	end_time = SDL_GetTicks();

	return ms;
}

void Engine::checkInput() {
    while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			prgRun = false;
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
                case SDLK_ESCAPE: prgRun = false; break;
                case SDLK_KP_MULTIPLY: showFPS = !showFPS; break;
                case SDLK_m: audio.unPause_Music(); break;

                case SDLK_f:
                    resources.getCFG().fullscreen ? resources.setChangeFullScreen(false) : resources.setChangeFullScreen(true);
                    eWindow->changeScreen(resources.getCFG().fullscreen);
                    resources.saveCFG();
                    break;

                case SDLK_v:
                    resources.getCFG().vsync ? resources.setChangeVSYNC(false) : resources.setChangeVSYNC(true);
                    eWindow->setVsync(resources.getCFG().vsync);
                    resources.saveCFG();
                    inChangeRenderer = true;
                    break;

                case SDLK_F5: setCharacterFx(0, 1, 1); break;
                case SDLK_o: charlist[0].invisible = !charlist[0].invisible; break;
			}
        }
    }
}

void Engine::checkKeys() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (UserMoving) return;

    if (keys[SDL_GetScancodeFromKey(SDLK_UP)]) {
        moveTo(eHeading::NORTH);
    } else if (keys[SDL_GetScancodeFromKey(SDLK_DOWN)]) {
        moveTo(eHeading::SOUTH);
    } else if (keys[SDL_GetScancodeFromKey(SDLK_LEFT)]) {
        moveTo(eHeading::WEST);
    } else if (keys[SDL_GetScancodeFromKey(SDLK_RIGHT)]) {
        moveTo(eHeading::EAST);
    }
}

void Engine::drawTextureGrhIndex(int grhIndex, int x, int y, Uint8 r, Uint8 g, Uint8 b, double angle, bool blend, Uint8 alpha) {
    grh graf;
    graf.grhIndex = grhIndex;
    resources.initGrh(graf, graf.grhIndex, false);

    if (graf.grhIndex == 0) return;

    SDL_Rect RECT; // creamos un recorte

    surface.checkTextureExist(renderer, resources.getGrhData(graf.grhIndex).filenum); // obtenemos una posicion indicada.
    if (blend) surface.gTextura()->textureLoaded.setBlend(SDL_BLENDMODE_ADD);
    surface.gTextura()->textureLoaded.setAlpha(alpha);
    surface.gTextura()->textureLoaded.setColor(r, g, b);

    RECT.x = resources.getGrhData(graf.grhIndex).sX;
    RECT.y = resources.getGrhData(graf.grhIndex).sY;
    RECT.w = resources.getGrhData(graf.grhIndex).pixelWidth;
    RECT.h = resources.getGrhData(graf.grhIndex).pixelHeight;

    surface.gTextura()->textureLoaded.render(renderer, x, y, &RECT, angle, NULL);
}

void Engine::drawTexture(grh& Grh, int x, int y, bool center, bool animate, bool blend, Uint8 alpha, double angle) {
    if (Grh.grhIndex == 0) return;
    if (resources.getGrhData(Grh.grhIndex).numFrames == 0) return;

    if (animate) {
        if (Grh.started) {
            Grh.frameCounter = Grh.frameCounter + (timerElapsedTime * resources.getGrhData(Grh.grhIndex).numFrames / Grh.speed);
            if (Grh.frameCounter > resources.getGrhData(Grh.grhIndex).numFrames) {
                Grh.frameCounter = 0;
                if (Grh.loops != -1) {
                    if (Grh.loops > 0) {
                        Grh.loops--;
                    } else {
                        Grh.started = false;
                    }
                }
            }
        }
    }

    int currentGrhIndex = resources.getGrhData(Grh.grhIndex).frames[(int)(Grh.frameCounter)];

    if (center) {
        if (resources.getGrhData(currentGrhIndex).tileWidth != 1) {
            x = x - (int)(resources.getGrhData(currentGrhIndex).tileWidth * TILE_PIXEL_SIZE / 2) + TILE_PIXEL_SIZE / 2;
        }

        if (resources.getGrhData(currentGrhIndex).tileHeight != 1) {
            y = y - (int)(resources.getGrhData(currentGrhIndex).tileHeight * TILE_PIXEL_SIZE) + TILE_PIXEL_SIZE;
        }
    }

    if (currentGrhIndex == 0) return;
    if (resources.getGrhData(currentGrhIndex).filenum == 0) return;

    SDL_Rect RECT; // creamos un recorte
    surface.checkTextureExist(renderer, resources.getGrhData(currentGrhIndex).filenum);
    if (blend) surface.gTextura()->textureLoaded.setBlend(SDL_BLENDMODE_ADD);
    surface.gTextura()->textureLoaded.setAlpha(alpha);

    RECT.x = resources.getGrhData(currentGrhIndex).sX;
    RECT.y = resources.getGrhData(currentGrhIndex).sY;
    RECT.w = resources.getGrhData(currentGrhIndex).pixelWidth;
    RECT.h = resources.getGrhData(currentGrhIndex).pixelHeight;

    surface.gTextura()->textureLoaded.render(renderer, x, y, &RECT, angle, NULL);
}

void Engine::drawText(std::string text, int x, int y, Uint8 r, Uint8 g, Uint8 b, int nType) {
    int ascii = 0, d = 0;
    if (text == "") return;
    const char* cText = text.c_str();

    for (int a = 0; a < (int)(strlen(cText)); a++) {
        ascii = (int)cText[a]; // consigo el ascii.
        if (ascii > 255) ascii = 0;

        if (ascii != 32) { // espacio
            if (resources.getFont(nType).ascii_code[ascii] != 0) {
                drawTextureGrhIndex(resources.getFont(nType).ascii_code[ascii], (x + d), y, r, g, b);
                d = d + resources.getGrhData(resources.getFont(nType).ascii_code[ascii]).pixelWidth + 1;
            }
        } else {
            d = d + 4;
        }
    }
}

void Engine::drawQuad(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool line) {
    SDL_Rect RECT;

    RECT.h = h;
    RECT.w = w;
    RECT.x = x;
    RECT.y = y;

    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    if (line) {
        SDL_RenderDrawRect(renderer, &RECT);
    } else {
        SDL_RenderFillRect(renderer, &RECT);
    }
}

void Engine::drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Engine::makeChar(int charIndex, eHeading heading, int x, int y) {
    charlist[charIndex].name = "Morgolock";
    charlist[charIndex].clan = "<Argentum Online Staff>";
    charlist[charIndex].pie = true;
    charlist[charIndex].priv = 2;
    charlist[charIndex].Body = resources.getBodys(0);
    charlist[charIndex].Head = resources.getHeads(0);
    charlist[charIndex].Weapon = resources.getWeapons(11);
    charlist[charIndex].Shield = resources.getShields(1);
    charlist[charIndex].Helmet = resources.getHelmets(1);
    charlist[charIndex].Heading = heading;
    charlist[charIndex].moving = false;
    charlist[charIndex].moveOffSetX = 0.0f;
    charlist[charIndex].moveOffSetY = 0.0f;
    charlist[charIndex].Pos.x = x;
    charlist[charIndex].Pos.y = y;
    charlist[charIndex].active = 1;
    charlist[charIndex].invisible = false;
    charlist[charIndex].fxIndex = -1;
    resources.setCharMap(x, y, charIndex);
}

void Engine::moveScreen(eHeading heading) {
    int X = 0; int Y = 0;
    int tX = 0; int tY = 0;

    switch (heading){
        case eHeading::NORTH: Y = -1; break;
        case eHeading::EAST: X = 1; break;
        case eHeading::SOUTH: Y = 1; break;
        case eHeading::WEST: X = -1; break;
    }

    tX = User.x + X;
    tY = User.y + Y;

    if (tX < 1 || tX > 98 || tY < 1 || tY > 98){
        return;
    } else {
        AddToUserPos.X = X;
        User.x = tX;
        AddToUserPos.Y = Y;
        User.y = tY;
        UserMoving = true;
    }
}

void Engine::moveCharByHead(int charIndex, eHeading heading) {
    int addX = 0, addY = 0;

    int X = charlist[charIndex].Pos.x,
        Y = charlist[charIndex].Pos.y;

    int nX, nY;

    switch (heading){
        case eHeading::NORTH: addY = -1; break;
        case eHeading::EAST: addX = 1; break;
        case eHeading::SOUTH: addY = 1; break;
        case eHeading::WEST: addX = -1; break;
    }

    nX = X + addX;
    nY = Y + addY;

    resources.setCharMap(X, Y, -1);
    resources.setCharMap(nX, nY, charIndex);

    charlist[charIndex].Pos.x = nX;
    charlist[charIndex].Pos.y = nY;

    charlist[charIndex].moveOffSetX = -1 * (TILE_PIXEL_SIZE * addX);
    charlist[charIndex].moveOffSetY = -1 * (TILE_PIXEL_SIZE * addY);

    charlist[charIndex].moving = true;
    charlist[charIndex].Heading = heading;

    charlist[charIndex].scrollDirectionX = addX;
    charlist[charIndex].scrollDirectionY = addY;

    doPasosFX(charIndex);
}

void Engine::doPasosFX(int charIndex) {
    charlist[charIndex].pie = !charlist[charIndex].pie;

    if (charlist[charIndex].pie) {
        audio.playSound("23");
    } else {
        audio.playSound("24");
    }
}

bool Engine::inMapBounds(int x, int y) {
    return (x <= 0 || x >= 99 || y <= 0 || y >= 99);
}

bool Engine::moveToLegalPos(int x, int y) {
    int CharIndex = resources.getMapData(x, y).charindex;

    /** Limite del mapa */
    if (inMapBounds(x, y)) return false;

    /** Tile Bloqueado? */
    if (resources.getMapData(x, y).blocked == 1) return false;

    // No puedo intercambiar con un casper que este en la orilla (Lado tierra)
    if (hayAgua(User.x, User.y)) {
        if (!hayAgua(x, y)) return false;
    } else {
        // No puedo intercambiar con un casper que este en la orilla (Lado agua)
        if (hayAgua(x, y)) return false;
    }

    return true;
}

void Engine::moveTo(eHeading Direccion) {
    bool LegalOk = false;

    charlist[0].Heading = Direccion;

    switch (Direccion) {
        case eHeading::NORTH:
            LegalOk = moveToLegalPos(User.x, User.y - 1);
            break;
        case eHeading::EAST:
            LegalOk = moveToLegalPos(User.x + 1, User.y);
            break;
        case eHeading::SOUTH:
            LegalOk = moveToLegalPos(User.x, User.y + 1);
            break;
        case eHeading::WEST:
            LegalOk = moveToLegalPos(User.x - 1, User.y);
            break;
    }

    if (LegalOk) {
        moveScreen(Direccion);
        moveCharByHead(0, Direccion);
    }
}

bool Engine::hayAgua(int x, int y) {
    return ((resources.getMapData(x, y).layer[0].grhIndex >= 1505 && resources.getMapData(x, y).layer[0].grhIndex <= 1520) ||
    (resources.getMapData(x, y).layer[0].grhIndex >= 5665 && resources.getMapData(x, y).layer[0].grhIndex <= 5680) ||
    (resources.getMapData(x, y).layer[0].grhIndex >= 13547 && resources.getMapData(x, y).layer[0].grhIndex <= 13562)) &&
        resources.getMapData(x, y).layer[1].grhIndex == 0;
}

int Engine::text_Width(std::string text) {
    int returnVal = 0, b = 0;
    const char* cText = text.c_str();

    if (strlen(cText) == 0) return 0;

    for (int a = 0; a < (int)(strlen(cText)); a++) {
        b = (int)cText[a]; // consigo el ascii.

        if ((b != 32) && (b != 5) && (b != 129) && (b != 9) && (b != 4) && (b != 255) && (b != 2) && (b != 151) && (b != 152)) {
            returnVal += resources.getGrhData(resources.getFont(0).ascii_code[b]).pixelWidth;
        } else {
            returnVal += 4;
        }
    }

    return returnVal;
}

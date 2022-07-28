#include "resources.h"

Resources::Resources() {

}

Resources::~Resources() {

}

bool Resources::loadResources() {
    if (!loadCFG()) return false;
    if (!loadGrhData()) return false;
    if (!loadBodys()) return false;
    if (!loadHeads()) return false;
    if (!loadHelmets()) return false;
    if (!loadWeapons()) return false;
    if (!loadShields()) return false;
    if (!loadFXs()) return false;
    if (!loadFonts()) return false;
    if (!loadMap(1)) return false;

    return true;
}

bool Resources::loadFonts() {
    SDL_RWops* file = SDL_RWFromFile("Resources/fonts.bin", "r+b");

    if (file == NULL){
        Error::logError("Cannot to open the file 'fonts.bin'");
        return false;
    }

    for (int i = 0; i < 255; i++) {
        SDL_RWread(file, &font_types[0].ascii_code[i], sizeof(int), 1);
        SDL_RWread(file, &font_types[1].ascii_code[i], sizeof(int), 1);
    }

    SDL_RWclose(file);
    return true;
}

bool Resources::loadBodys() {
    short numBodys;

    SDL_RWops* file = SDL_RWFromFile("Resources/bodys.ind", "r+b");

    if (file == NULL) {
        Error::logError("Cannot to open the file 'bodys.ind'");
        return false;
    }

    SDL_RWread(file, &cabecera, sizeof(cabecera), 1);
    SDL_RWread(file, &numBodys, sizeof(numBodys), 1);

    BodyData = new bodyData[numBodys];

    for (int i = 0; i < numBodys; i++) {
        SDL_RWread(file, &BodyData[i].walk[0].grhIndex, sizeof(BodyData[i].walk[0].grhIndex), 1);
        SDL_RWread(file, &BodyData[i].walk[1].grhIndex, sizeof(BodyData[i].walk[1].grhIndex), 1);
        SDL_RWread(file, &BodyData[i].walk[2].grhIndex, sizeof(BodyData[i].walk[2].grhIndex), 1);
        SDL_RWread(file, &BodyData[i].walk[3].grhIndex, sizeof(BodyData[i].walk[3].grhIndex), 1);

        SDL_RWread(file, &BodyData[i].headOffsetX, sizeof(BodyData[i].headOffsetX), 1);
        SDL_RWread(file, &BodyData[i].headOffsetY, sizeof(BodyData[i].headOffsetY), 1);

        if (BodyData[i].walk[0].grhIndex != 0){
            initGrh(BodyData[i].walk[0], BodyData[i].walk[0].grhIndex, false);
            initGrh(BodyData[i].walk[1], BodyData[i].walk[1].grhIndex, false);
            initGrh(BodyData[i].walk[2], BodyData[i].walk[2].grhIndex, false);
            initGrh(BodyData[i].walk[3], BodyData[i].walk[3].grhIndex, false);
        }
    }

    SDL_RWclose(file);
    return true;
}

bool Resources::loadHeads() {
    short numHeads;

    SDL_RWops* file = SDL_RWFromFile("Resources/heads.ind", "r+b");

    if (file == NULL) {
        Error::logError("Cannot to open the file 'heads.ind'");
        return false;
    }

    SDL_RWread(file, &cabecera, sizeof(cabecera), 1);
    SDL_RWread(file, &numHeads, sizeof(numHeads), 1);

    HeadData = new hhwsData[numHeads];

    for (int i = 0; i < numHeads; i++) {
        SDL_RWread(file, &HeadData[i].walk[0].grhIndex, sizeof(HeadData[i].walk[0].grhIndex), 1);
        SDL_RWread(file, &HeadData[i].walk[1].grhIndex, sizeof(HeadData[i].walk[1].grhIndex), 1);
        SDL_RWread(file, &HeadData[i].walk[2].grhIndex, sizeof(HeadData[i].walk[2].grhIndex), 1);
        SDL_RWread(file, &HeadData[i].walk[3].grhIndex, sizeof(HeadData[i].walk[3].grhIndex), 1);

        if (HeadData[i].walk[0].grhIndex != 0) {
            initGrh(HeadData[i].walk[0], HeadData[i].walk[0].grhIndex, false);
            initGrh(HeadData[i].walk[1], HeadData[i].walk[1].grhIndex, false);
            initGrh(HeadData[i].walk[2], HeadData[i].walk[2].grhIndex, false);
            initGrh(HeadData[i].walk[3], HeadData[i].walk[3].grhIndex, false);
        }
    }

    SDL_RWclose(file);
    return true;
}

bool Resources::loadHelmets() {
    short numHelmets;

    SDL_RWops* file = SDL_RWFromFile("Resources/helmets.ind", "r+b");

    if (file == NULL) {
        Error::logError("Cannot to open the file 'helmets.ind'");
        return false;
    }

    SDL_RWread(file, &cabecera, sizeof(cabecera), 1);
    SDL_RWread(file, &numHelmets, sizeof(numHelmets), 1);

    HelmetsData = new hhwsData[numHelmets];

    for (int i = 0; i < numHelmets; i++) {
        SDL_RWread(file, &HelmetsData[i].walk[0].grhIndex, sizeof(HelmetsData[i].walk[0].grhIndex), 1);
        SDL_RWread(file, &HelmetsData[i].walk[1].grhIndex, sizeof(HelmetsData[i].walk[1].grhIndex), 1);
        SDL_RWread(file, &HelmetsData[i].walk[2].grhIndex, sizeof(HelmetsData[i].walk[2].grhIndex), 1);
        SDL_RWread(file, &HelmetsData[i].walk[3].grhIndex, sizeof(HelmetsData[i].walk[3].grhIndex), 1);

        if (HelmetsData[i].walk[0].grhIndex != 0) {
            initGrh(HelmetsData[i].walk[0], HelmetsData[i].walk[0].grhIndex, false);
            initGrh(HelmetsData[i].walk[1], HelmetsData[i].walk[1].grhIndex, false);
            initGrh(HelmetsData[i].walk[2], HelmetsData[i].walk[2].grhIndex, false);
            initGrh(HelmetsData[i].walk[3], HelmetsData[i].walk[3].grhIndex, false);
        }
    }

    SDL_RWclose(file);
    return true;
}

bool Resources::loadShields() {
    short numShields;

    SDL_RWops* file = SDL_RWFromFile("Resources/shields.ind", "r+b");

    if (file == NULL) {
        Error::logError("Cannot to open the file 'shields.ind'");
        return false;
    }

    SDL_RWread(file, &numShields, sizeof(numShields), 1);

    ShieldData = new hhwsData[numShields];

    for (int i = 0; i < numShields; i++) {
        SDL_RWread(file, &ShieldData[i].walk[0].grhIndex, sizeof(ShieldData[i].walk[0].grhIndex), 1);
        SDL_RWread(file, &ShieldData[i].walk[1].grhIndex, sizeof(ShieldData[i].walk[1].grhIndex), 1);
        SDL_RWread(file, &ShieldData[i].walk[2].grhIndex, sizeof(ShieldData[i].walk[2].grhIndex), 1);
        SDL_RWread(file, &ShieldData[i].walk[3].grhIndex, sizeof(ShieldData[i].walk[3].grhIndex), 1);

        if (ShieldData[i].walk[0].grhIndex != 0) {
            initGrh(ShieldData[i].walk[0], ShieldData[i].walk[0].grhIndex, false);
            initGrh(ShieldData[i].walk[1], ShieldData[i].walk[1].grhIndex, false);
            initGrh(ShieldData[i].walk[2], ShieldData[i].walk[2].grhIndex, false);
            initGrh(ShieldData[i].walk[3], ShieldData[i].walk[3].grhIndex, false);
        }
    }

    SDL_RWclose(file);
    return true;
}

bool Resources::loadWeapons() {
    short numWeapons;

    SDL_RWops* file = SDL_RWFromFile("Resources/weapons.ind", "r+b");

    if (file == NULL) {
        Error::logError("Cannot to open the file 'weapons.ind'");
        return false;
    }

    SDL_RWread(file, &numWeapons, sizeof(numWeapons), 1);

    WeaponData = new hhwsData[numWeapons];

    for (int i = 0; i < numWeapons; i++) {
        SDL_RWread(file, &WeaponData[i].walk[0].grhIndex, sizeof(WeaponData[i].walk[0].grhIndex), 1);
        SDL_RWread(file, &WeaponData[i].walk[1].grhIndex, sizeof(WeaponData[i].walk[1].grhIndex), 1);
        SDL_RWread(file, &WeaponData[i].walk[2].grhIndex, sizeof(WeaponData[i].walk[2].grhIndex), 1);
        SDL_RWread(file, &WeaponData[i].walk[3].grhIndex, sizeof(WeaponData[i].walk[3].grhIndex), 1);

        if (WeaponData[i].walk[0].grhIndex != 0) {
            initGrh(WeaponData[i].walk[0], WeaponData[i].walk[0].grhIndex, false);
            initGrh(WeaponData[i].walk[1], WeaponData[i].walk[1].grhIndex, false);
            initGrh(WeaponData[i].walk[2], WeaponData[i].walk[2].grhIndex, false);
            initGrh(WeaponData[i].walk[3], WeaponData[i].walk[3].grhIndex, false);
        }
    }

    SDL_RWclose(file);
    return true;
}

bool Resources::loadCFG() {
    SDL_RWops* file = SDL_RWFromFile("Resources/cfg.bin", "r+b");

    if (file == NULL) 
    {
        Error::logError("Cannot to open the file 'cfg.bin'");
        return false;
    }

    SDL_RWread(file, &gCFG.screen_width, sizeof(int), 1);
    SDL_RWread(file, &gCFG.screen_height, sizeof(int), 1);
    SDL_RWread(file, &gCFG.volume, sizeof(int), 1);
    SDL_RWread(file, &gCFG.fullscreen, sizeof(bool), 1);
    SDL_RWread(file, &gCFG.capFPS, sizeof(bool), 1);
    SDL_RWread(file, &gCFG.vsync, sizeof(bool), 1);

    SDL_RWclose(file);

    gCFG.screen_width = 800;
    gCFG.screen_height = 600;

    return true;
}

void Resources::saveCFG() {
    SDL_RWops* file = SDL_RWFromFile("Resources/cfg.bin", "w+b");

    if (file == NULL) {
        Error::logError("Cannot to open the file 'cfg.bin'");
        return;
    }

    SDL_RWwrite(file, &gCFG.screen_width, sizeof(int), 1);
    SDL_RWwrite(file, &gCFG.screen_height, sizeof(int), 1);
    SDL_RWwrite(file, &gCFG.volume, sizeof(int), 1);
    SDL_RWwrite(file, &gCFG.fullscreen, sizeof(bool), 1);
    SDL_RWwrite(file, &gCFG.capFPS, sizeof(bool), 1);
    SDL_RWwrite(file, &gCFG.vsync, sizeof(bool), 1);

    SDL_RWclose(file);
}

bool Resources::loadMap(int numMap) {
    std::stringstream path;
    path << "Resources/maps/mapa" << numMap << ".map";

    SDL_RWops* file = SDL_RWFromFile(path.str().c_str(), "r+b");
    
    if (file == NULL) {
        std::stringstream msgError;
        msgError << "Error al cargar el mapa" << numMap << ".map";
        Error::logError(msgError.str().c_str());
        return false;
    }

    short mapversion, tempint;
    char byflags;

    SDL_RWread(file, &mapversion, sizeof(mapversion), 1);
    SDL_RWread(file, &cabecera, sizeof(cabecera), 1);

    SDL_RWread(file, &tempint, sizeof(tempint), 1);
    SDL_RWread(file, &tempint, sizeof(tempint), 1);
    SDL_RWread(file, &tempint, sizeof(tempint), 1);
    SDL_RWread(file, &tempint, sizeof(tempint), 1);

    for (int y = 0; y < 100; y++){
        for (int x = 0; x < 100; x++){
            SDL_RWread(file, &byflags, sizeof(byflags), 1);

            if (byflags & 1) MapData[x][y].blocked = 1;

            SDL_RWread(file, &MapData[x][y].layer[0].grhIndex, sizeof(MapData[x][y].layer[0].grhIndex), 1);
            initGrh(MapData[x][y].layer[0], MapData[x][y].layer[0].grhIndex, true);
            //Engine::surface.ADD_NEW_TEXTURE(Engine::)
            

            if (byflags & 2){
                SDL_RWread(file, &MapData[x][y].layer[1].grhIndex, sizeof(MapData[x][y].layer[1].grhIndex), 1);
                initGrh(MapData[x][y].layer[1], MapData[x][y].layer[1].grhIndex, true);
            } else {
                MapData[x][y].layer[1].grhIndex = 0;
            }

            if (byflags & 4){
                SDL_RWread(file, &MapData[x][y].layer[2].grhIndex, sizeof(MapData[x][y].layer[2].grhIndex), 1);
                initGrh(MapData[x][y].layer[2], MapData[x][y].layer[2].grhIndex, true);
            } else {
                MapData[x][y].layer[2].grhIndex = 0;
            }

            if (byflags & 8) {
                SDL_RWread(file, &MapData[x][y].layer[3].grhIndex, sizeof(MapData[x][y].layer[3].grhIndex), 1);
                initGrh(MapData[x][y].layer[3], MapData[x][y].layer[3].grhIndex, true);
            } else {
                MapData[x][y].layer[3].grhIndex = 0;
            }

            if (byflags & 16) {
                SDL_RWread(file, &MapData[x][y].trigger, sizeof(MapData[x][y].trigger), 1);
            }


            if (MapData[x][y].charindex > -1) MapData[x][y].charindex = -1;

            MapData[x][y].objgrh.grhIndex = 0;
        }
    }

    SDL_RWclose(file);

    return true;
}

bool Resources::loadGrhData() {
    SDL_RWops* file = SDL_RWFromFile("Resources/graphics.ind", "r+b");

    if (file == NULL) {
        Error::logError("Cannot to open the file 'graphics.ind'");
        return false;
    }

    int grhCount, Grh = 0, tempint;
    float tempfloat;
    short tempshort;

    SDL_RWread(file, &tempint, sizeof(int), 1);
    SDL_RWread(file, &grhCount, sizeof(int), 1);

    GrhData = new grhData[grhCount];

    while (Grh < grhCount) {
        SDL_RWread(file, &Grh, sizeof(int), 1);
        SDL_RWread(file, &GrhData[Grh].numFrames, sizeof(short), 1);

        if (GrhData[Grh].numFrames <= 0) 
        {
            return false;
        }

        GrhData[Grh].frames = new int[GrhData[Grh].numFrames];

        if (GrhData[Grh].numFrames > 1) 
        {
            for (int frame = 0; frame < GrhData[Grh].numFrames; frame++) 
            {
                SDL_RWread(file, &GrhData[Grh].frames[frame], sizeof(int), 1);
                if (GrhData[Grh].frames[frame] <= 0) 
                {
                    return false;
                }
            }

            SDL_RWread(file, &GrhData[Grh].speed, sizeof(float), 1);
            if (GrhData[Grh].speed <= 0) 
            {
                return false;
            }

            GrhData[Grh].pixelHeight = GrhData[GrhData[Grh].frames[0]].pixelHeight;
            GrhData[Grh].pixelWidth = GrhData[GrhData[Grh].frames[0]].pixelWidth;
            GrhData[Grh].tileWidth = GrhData[GrhData[Grh].frames[0]].tileWidth;
            GrhData[Grh].tileHeight = GrhData[GrhData[Grh].frames[0]].tileHeight;

        }
        else 
        {

            SDL_RWread(file, &GrhData[Grh].filenum, sizeof(int), 1);
            if (GrhData[Grh].filenum <= 0) 
            {
                return false;
            }

            SDL_RWread(file, &GrhData[Grh].sX, sizeof(short), 1);
            if (GrhData[Grh].sX < 0) {
                return false;
            }

            SDL_RWread(file, &GrhData[Grh].sY, sizeof(short), 1);
            if (GrhData[Grh].sY < 0) 
            {
                return false;
            }

            SDL_RWread(file, &GrhData[Grh].pixelWidth, sizeof(short), 1);
            if (GrhData[Grh].pixelWidth <= 0) 
            {
                return false;
            }

            SDL_RWread(file, &GrhData[Grh].pixelHeight, sizeof(short), 1);
            if (GrhData[Grh].pixelHeight <= 0) 
            {
                return false;
            }

            GrhData[Grh].tileWidth = GrhData[Grh].pixelWidth / 32.0;
            GrhData[Grh].tileHeight = GrhData[Grh].pixelHeight / 32.0;
            GrhData[Grh].frames[0] = Grh;
        }
    }

    SDL_RWclose(file);
    return true;
}

bool Resources::loadFXs() {
    SDL_RWops* file = SDL_RWFromFile("Resources/fxs.ind", "r+b");

    if (file == NULL) 
    {
        Error::logError("Cannot to open the file 'fxs.ind'");
        return false;
    }
    
    short numFXs;

    SDL_RWread(file, &cabecera, sizeof(cabecera), 1);
    SDL_RWread(file, &numFXs, sizeof(numFXs), 1);

    FxData = new fxData[numFXs];

    for (int i = 0; i < numFXs; i++) 
    {
        SDL_RWread(file, &FxData[i], sizeof(FxData[i]), 1);
    }

    SDL_RWclose(file);

    return true;
}

void Resources::initGrh(grh& Grh, short grhindex, bool started) {
    Grh.grhIndex = grhindex;
    Grh.frameCounter = 0;

    if (started) 
    {
        GrhData[Grh.grhIndex].numFrames > 1 ? Grh.started = true : Grh.started = false;
    }
    else 
    {
        if (GrhData[Grh.grhIndex].numFrames == 1) started = false;
        Grh.started = started;
    }

    Grh.started ? Grh.loops = -1 : Grh.loops = 0;

    Grh.frameCounter = 0;
    Grh.speed = GrhData[Grh.grhIndex].speed;
}

Configuration& Resources::getCFG() 
{ 
    return gCFG;
}

grhData& Resources::getGrhData(int grhIndex) 
{
    return GrhData[grhIndex];
}

fonts& Resources::getFont(int nType) 
{
    return font_types[nType];
}

mapData& Resources::getMapData(int x, int y) 
{
    return MapData[x][y];
}

bodyData& Resources::getBodys(short numBody) 
{
    return BodyData[numBody];
}

hhwsData& Resources::getHeads(short numHead)
{
    return HeadData[numHead];
}

hhwsData& Resources::getHelmets(short numHelmet) 
{
    return HelmetsData[numHelmet];
}

hhwsData& Resources::getShields(short numShields) 
{
    return ShieldData[numShields];
}

hhwsData& Resources::getWeapons(short numWeapons) 
{
    return WeaponData[numWeapons];
}

fxData& Resources::getFX(short numFX) 
{
    return FxData[numFX];
}

void Resources::setCharMap(int x, int y, int charIndex) 
{
    if ((x >= 0 && x <= 99) && (y >= 0 && y <= 99)) MapData[x][y].charindex = charIndex;
}

void Resources::setChangeFullScreen(bool activate) 
{
    gCFG.fullscreen = activate;
}

void Resources::setChangeVSYNC(bool activate) 
{
    gCFG.vsync = activate;
}
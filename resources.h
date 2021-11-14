#pragma once

#include <SDL.h>
#include <sstream>
#include <stdio.h>

struct Configuration {
	int screen_width, screen_height, volume;
	bool fullscreen, vsync, capFPS;
};

struct grhData {
    short sX;
    short sY;

    int filenum;

    short pixelWidth;
    short pixelHeight;

    float tileWidth;
    float tileHeight;

    short numFrames;
    int* frames;

    float speed;
};

struct grh {
    short grhIndex;
    float frameCounter;
    float speed;
    bool started;
    int loops;
    float angle;
};

struct fonts {
    int ascii_code[255];
};

struct obj {
    short objIndex;
    short amount;
};

struct worldPos {
    short map;
    short x;
    short y;
};

struct mapData {
    grh layer[4];
    short charindex;
    grh objgrh;
    short npcindex;
    obj objinfo;
    worldPos tileexit;
    char blocked;
    short trigger;
};


struct bodyData {
    grh walk[4];
    short headOffsetX;
    short headOffsetY;
};

struct hhwsData {
    grh walk[4];
};

struct fxData {
    short Animacion;
    short OffsetX;
    short OffsetY;
};

class Resources {
	private:
        char cabecera[263];
		Configuration gCFG;
        fonts font_types[2];
        mapData MapData[100][100];

        grhData* GrhData;
        bodyData* BodyData;
        hhwsData* HeadData;
        hhwsData* HelmetsData;
        hhwsData* ShieldData;
        hhwsData* WeaponData;
        fxData* FxData;

	public:
        Resources();
		~Resources();

        bool loadResources();
        bool loadCFG();
        bool loadGrhData();
        bool loadFonts();
        bool loadMap(int);
        bool loadBodys();
        bool loadHeads();
        bool loadHelmets();
        bool loadShields();
        bool loadWeapons();
        bool loadFXs();
        void saveCFG();

        void initGrh(grh&, short, bool);

        void setCharMap(int, int, int);
        void setChangeFullScreen(bool);
        void setChangeVSYNC(bool);

        Configuration& getCFG();
        grhData& getGrhData(int);
        fonts& getFont(int);
        mapData& getMapData(int, int);
        bodyData& getBodys(short);
        hhwsData& getHeads(short);
        hhwsData& getHelmets(short);
        hhwsData& getShields(short);
        hhwsData& getWeapons(short);
        fxData& getFX(short);
};
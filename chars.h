#pragma once
#include "resources.h"

enum eHeading { 
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct position{
    int x;
    int y;
};

struct addToUser{
    int X;
    int Y;
};

struct color {
    Uint8 r, g, b;
};

struct Char{
    std::string name, clan;

    eHeading Heading;
    position Pos;
    bodyData Body;
    hhwsData Head;
    hhwsData Helmet;
    hhwsData Weapon;
    hhwsData Shield;

    grh fX;
    short fxIndex;

    short scrollDirectionX, scrollDirectionY;
    char active, priv, criminal;
    float moveOffSetX, moveOffSetY;
    bool pie, dead, invisible, attackable, moving, usingArm;
};
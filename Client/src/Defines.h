#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#include <iostream>
#include "SDL.h"

#define FPS 60

// BASIC COLORS
const SDL_Color RED = {215,40,40,90};
const SDL_Color BLUE = {0,0,177,1};
const SDL_Color BLACK = {0,0,0,0};
const SDL_Color WHITE = {255,255,255,255};
const SDL_Color GREEN = {37,199,28,1};
const SDL_Color PURPLE = {131,28,199,1};
//

// STRUCTURES FOR THE GAME
struct button
{
    unsigned int index;
    int x1,x2,x3;
    int y1,y2,y3;
    int w,h;
    std::string picture;
};
struct background
{
    unsigned int index;
    std::string picture;
};
struct fonts
{
    unsigned int index;
    std::string font;
};
struct anime
{
    int index;
    int w,h;
    int startX;
    int startY;
    int frames;
    std::string picture;
};
//



#endif // DEFINES_H_INCLUDED

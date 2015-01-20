/*
 * Definitions.h
 *
 *  Created on: 22.11.2014 ã.
 *      Author: Niel-Home
 */

#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED
#include <stdio.h>
#include "SDL_image.h"
#include "SDL.h"

const int g_WINDOW_WIDTH = 1280;
const int g_WINDOW_HEIGHT = 720;


struct PanelInfo
{
    unsigned int width;
    unsigned int height;
    unsigned int left;
    unsigned int top;
};


#define TITLE "WIndow Test"
#define FPS 60


#endif /* DEFINITIONS_H_ */

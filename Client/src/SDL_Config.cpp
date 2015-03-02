#include"SDL_Config.h"

#include <iostream>
#include <fstream>
#include <string>
#include "Tools.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;

SDL_CONFIG::SDL_CONFIG()
{
    win_width = 0;
    win_height = 0;
}
SDL_CONFIG::~SDL_CONFIG()
{
    SDL_Quit();
    SDL_DestroyWindow(window);
}

void SDL_CONFIG::ReadWindowConfig(char* src)
{
    std::ifstream file(src);
    std::string line;
    if(file.is_open())
    {
        while( file >>line)
        {
            if(line == "win_width")
            {
                file >> line;
                file >> line;
                win_width = StrToInt(line);
            }
            if(line == "win_height")
            {
                file >> line;
                file >> line;
                win_height = StrToInt(line);
            }
            if(line == "win_name")
            {
                file >> line;
                file >> line;
                win_name = line.c_str();
            }

        }
    }
    else
        std::cout << "Unable to open " << src << "file" << std::endl;
    file.close();
}

bool SDL_CONFIG::Init_Window()
{
    ReadWindowConfig("data/win_config.ini");
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("INITIALIZE fail: %s \n", SDL_GetError());
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
    {
        printf("Warning: VSync not enabled!\n");
        return false;
    }
    window = SDL_CreateWindow(win_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height,SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        printf("ERROR Window not create : %s \n ", SDL_GetError());
        return false;
    }

    render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(NULL == render)
    {
        printf("Failed creating Render : %s\n",SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor( render, 0xFF, 0xFF, 0xFF, 0xFF );

    int picFlag = IMG_INIT_PNG;
    if(!(IMG_Init(picFlag))& picFlag)
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        printf("Warning: Scale Quality not enabled!\n");
        return false;
    }
    if(TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n",TTF_GetError());
        return false;
    }

    if(!ButtonReadCSV("Buttons.csv"))
    {
        return false;
    }

    if(!BackGroudsReadCSV("BackGrounds.csv"))
    {
        return false;
    }

    if(!FontsReadCSV("Fonts.csv"))
    {
        return false;
    }
    TestReadCSV("Ninja.csv");
    return true;
}
















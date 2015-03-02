#ifndef SDL_CONFIG_H_INCLUDED
#define SDL_CONFIG_H_INCLUDED

#include "SDL.h"
#include <string>

extern SDL_Window* window;
extern SDL_Renderer* render;
extern SDL_Event game_event;

class SDL_CONFIG
{
private:
    unsigned int win_width,win_height;
    std::string win_name;
public:
    SDL_CONFIG();
    ~SDL_CONFIG();
    bool Init_Window();
    void ReadWindowConfig(char* src);

};


#endif // SDL_CONFIG_H_INCLUDED

#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED
#include <iostream>

#include "SDL.h"
#include "Tools.h"
#include "SDL_Config.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <vector>
#include <map>


class GraphicManager
{
public:
    bool load(std::string src,std::string id);
    void draw(std::string id,int x,int y,int w,int h,SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string id,int x,int y,int w,int h,int dW,int dH,int curRow,int curFrame,double angle,int alpha,SDL_RendererFlip flip = SDL_FLIP_NONE);
    void clearTextureFromMap(std::string id);
    void Clean();

    void drawText(std::string font_src,std::string text,int x,int y,int w,int h,SDL_Color col,int font_size);

    static GraphicManager* Instance()
    {
        if(s_Instansce == 0)
        {
            s_Instansce = new GraphicManager();
            return s_Instansce;
        }
        return s_Instansce;
    }

private:
    GraphicManager()
    {
    }
    static GraphicManager* s_Instansce;
    std::map<std::string, SDL_Texture*> m_Textures;
};

typedef GraphicManager GM;
/*
    - FPS are default  10 frames per second;
*/


#endif // GRAPHIC_H_INCLUDED

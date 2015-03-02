#include "Graphic.h"


GraphicManager* GraphicManager::s_Instansce = 0;

bool GraphicManager::load(std::string src,std::string id)
{
    SDL_Surface* pTempSurface = IMG_Load(src.c_str());
    if(pTempSurface == 0)
    {
        return false;
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(render,pTempSurface);
    SDL_FreeSurface(pTempSurface);
    if(pTexture != 0)
    {
        m_Textures[id] = pTexture;
        return true;
    }
    return false;
}

void GraphicManager::draw(std::string id,int x,int y,int w,int h,SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect destRect;
    //srcRect = {0,0,w,h};
    destRect = {x,y,w,h};

    SDL_RenderCopyEx(render,m_Textures[id],NULL,&destRect,0,0,flip);
}

void GraphicManager::drawFrame(std::string id,int x,int y,int w,int h,int dW,int dH,int curRow,int curFrame,double angle,int alpha,SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect = {w*curFrame,h*curRow,w,h};
    destRect = {x,y,dW,dH};
    SDL_SetTextureAlphaMod(m_Textures[id],alpha);
    SDL_RenderCopyEx(render,m_Textures[id],&srcRect,&destRect,angle,0,flip);
}

void GraphicManager::clearTextureFromMap(std::string id)
{
    SDL_DestroyTexture(m_Textures[id]);
    m_Textures.erase(id);
}

void GraphicManager::drawText(std::string font_src,std::string text,int x,int y,int w,int h,SDL_Color col,int font_size)
{
    SDL_Rect dest = {x,y,w,font_size};

    SDL_Color color = col;
    int text_length = text.length()+1;

    TTF_Font* font = TTF_OpenFont(font_src.c_str(),font_size);
    if(!font)
    {
        std::cout << TTF_GetError();
    }

    dest.w = text_length*font_size - 1;
    SDL_Rect src = dest;
    if(dest.w >= w)
    {
        dest.w = w;
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended(font,text.c_str(),color);
    SDL_Texture*textTexture = SDL_CreateTextureFromSurface(render,textSurface);
    SDL_RenderCopy(render,textTexture,NULL,&dest);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void GraphicManager::Clean()
{
    if(!m_Textures.empty())
    {
        for(std::map<std::string,SDL_Texture*>::iterator it = m_Textures.begin();it!= m_Textures.end();it++)
        {
            SDL_DestroyTexture((*it).second);
        }
        m_Textures.clear();
    }
}





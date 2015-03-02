#include <iostream>
#include "src/SDL_Config.h"
#include "src/Game.h"
#include "src/BSDv2.h"

ComCon *ComCon::m_pInstance = NULL;

int main(int argc, char **argv)
{
    ComCon::Instance()->Init();
    SDL_CONFIG* WindowConfig = new SDL_CONFIG();
    WindowConfig->Init_Window();
    G::Instance()->Init();
    Uint32 time;
    while(G::Instance()->running())
    {
        time = SDL_GetTicks();
        SDL_SetRenderDrawColor(render,255,255,255,0);
        SDL_RenderClear(render);

        G::Instance()->handleEvents();
        G::Instance()->Update();
        G::Instance()->Draw();

        SDL_RenderPresent(render);

        if(SDL_GetTicks() - time < 1000/FPS)
        {
            SDL_Delay(1000/FPS - (SDL_GetTicks() - time));
        }
    }
    std::cout << "LEAVING\n";
    delete WindowConfig;
    return 0;
}

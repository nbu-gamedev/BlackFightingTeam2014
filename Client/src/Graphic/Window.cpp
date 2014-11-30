/*
 * Window.cpp
 *
 *  Created on: 22.11.2014 ã.
 *      Author: Niel-Home
 */
#include "Window.h"

Window::Window()
 {
	m_Window=NULL;
	m_render=NULL;
 }
Window::~Window()
{
	SDL_DestroyRenderer(m_render);
	SDL_DestroyWindow(m_Window);
}
bool Window::InitWindow()
{
	bool init=true;
		if(SDL_Init(SDL_INIT_EVERYTHING)<0)
		{
			init=false;
			printf("INITIALIZE fail: %s \n", SDL_GetError());
		}
		else{
			printf("Initialized \n");

			if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
			            {
			                printf("Warning: VSync not enabled!\n");
			                init = false;
			            }

			m_Window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_WINDOW_WIDTH, g_WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
			if(m_Window==NULL)
			{
				printf("ERROR Window not create : %s \n ", SDL_GetError());
				init=false;
			}
			else
			{
				printf("Window created \n");

				m_render = SDL_CreateRenderer(m_Window,-1,SDL_RENDERER_ACCELERATED);

				if(m_render == NULL)
				  {
				    printf("Failed creating Render : %s\n",SDL_GetError());
				    init= false;
				  }
				else
				{
					SDL_SetRenderDrawColor( m_render, 0xFF, 0xFF, 0xFF, 0xFF );

					   int picFlag = IMG_INIT_PNG;
					   if(!(IMG_Init(picFlag))& picFlag)
					     {
					       printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					       init = false;
					     }
					   else
					     {
					       if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
					        {
					          printf("Warning: Scale Quality not enabled!\n");
					           init = false;
					      }
				}
			}

		}
	}

return init;

};







#include "SDL.h"
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Definitions.h"
#include "Window.h"
#include "Graphic.h"
#include "Elements.h"
Picture new_img;
Window* m_window;
Container cn;
Panel c_panel;
int MouseX, MouseY;
int posx=0, posy=0;
bool more=false;
void Change()
{
	if(more){
	posx=MouseX;
	posy=MouseY;
	}
}
void Init()
{
	m_window=new Window();


	m_window->InitWindow();
	new_img.Init(m_window->m_render,"Art/rectangle.png" );
	c_panel.Init( 300, 10, 100, 100);
	c_panel.ColorBorder[0]=200;
	c_panel.ColorBorder[1]=255;
	c_panel.ColorBorder[2]=234;
	c_panel.ColorBorder[3]=255;
	cn.Append(&c_panel);
}
void Draw()
{
	SDL_SetRenderDrawColor( m_window->m_render, 0x00, 0x00, 0x00, 0xFF );
	SDL_RenderClear(m_window->m_render);

	DrawBox(m_window->m_render, 100, 50, 200, 150);
	//DrawBox(m_window->m_render, posx, posy, 40, 150, 255,0,255,255);
	//FillBox(m_window->m_render, 100, 50, 200, 150, 0,255,255,255);
	//FillBox(m_window->m_render, 0, 0, 40, 150, 255,0,255,255);
	//DrawCircle(m_window->m_render, 170, 300, 50, 255,0,5,255);
	c_panel.Draw(m_window->m_render);
	cn.c_Draw(m_window->m_render);
	new_img.Draw(30, 50,50, 50,0.0, m_window->m_render);
	SDL_RenderPresent(m_window->m_render);

}
int main(int argc, char* args[])
{
	SDL_Event ev;
	Init();


	unsigned int starttime;
	unsigned int currenttime;
	bool run =true;
	while(run)
	{
		starttime=SDL_GetTicks();
		if(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_MOUSEMOTION)
			{
				SDL_GetMouseState(&MouseX,&MouseY);
				//printf("position x - %i\n", MouseX);
				//printf("position y - %i\n", MouseY);
			}
			if(ev.type == SDL_MOUSEBUTTONDOWN)
			{
				more=true;
			}
			if(ev.type == SDL_MOUSEBUTTONUP)
			{
				more=false;
			 }
			 if(ev.key.keysym.sym == SDLK_ESCAPE)
			 {

			 }
			 if(ev.type == SDL_QUIT)
			 {
			   run = false;
			   break;
			 }
			 Change();
		}
		Draw();

	currenttime=SDL_GetTicks();
	if(1000/FPS > currenttime- starttime)
	{
		SDL_Delay(1000/FPS-(currenttime-starttime));
	}
	}
	delete m_window;
	return 0;
}

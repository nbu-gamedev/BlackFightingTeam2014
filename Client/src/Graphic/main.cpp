#include "SDL.h"
#include <SDL_image.h>
#include <stdio.h>
#include "stdlib.h"
#include "iostream"
#include "fstream"
#include "string.h"
#include <string>
#include <vector>
#include "Definitions.h"
#include "Window.h"
#include "Graphic.h"
#include "Elements.h"

Picture new_img;
Window* m_window;
Container cn;
Panel c_panel, c_panel2;
Button t_button, t_button2, t_button3;
Slider s_button;
ButtonImg newbimg;
DrawObjects Objs;
int MouseX, MouseY;
int posx=0, posy=0;
bool more=false;
bool run =true;
float val=20;
void Change()
{
	if(more){
	posx=MouseX;
	posy=MouseY;
	}
}
void Exit(){
run=!run;
}
void readFile(std::string filename){

}
void Init()
{
	m_window=new Window();
readFile("File3.csv");
	m_window->InitWindow();
	new_img.Init(m_window->m_render,"Art/rectangle.png" );
	cn.Init(0,0,g_WINDOW_WIDTH, g_WINDOW_HEIGHT, m_window->m_render);
	c_panel.Init( 20, 20, 400, 500, m_window->m_render,Aline_normal,OwnWidth, dPicture,"Art/bg.png");
	c_panel.m_colorborder={200,255,234,255};
	c_panel.m_colorfill={200,255,234,255};
	c_panel.numbre=1;
	c_panel.readFile("File3.csv");
	c_panel2.Init(4000, 2000, 100, 100, m_window->m_render);
	c_panel2.m_colorborder={200,255,234,255};
	c_panel2.m_colorfill={200,255,234,255};
	c_panel2.numbre=2;
	t_button.Init(0, 0, 50, 50, m_window->m_render);
	t_button.m_colorborder={200,23,26,255};
	t_button.m_colorfill={200,23,26,255};
	t_button.m_colorhover={10,200,100,255};
	t_button.m_colorclick={100,20,200,255};
	t_button.m_pOnClick=Exit;
    t_button.numbre=1;

	t_button2.Init(10, 100, 50, 50, m_window->m_render, Aline_normal, OwnWidth, dPicture, "Art/button.png");
	t_button2.m_colorborder={200,23,26,255};
	t_button2.m_colorfill={200,23,26,255};
	t_button2.m_colorhover={10,200,100,255};
	t_button2.m_colorclick={100,20,200,255};
	t_button2.numbre=2;

    t_button2.Copy(t_button3);
    t_button3.m_Top=200;
	t_button3.m_colorborder={200,23,26,255};
	t_button3.m_colorfill={200,23,26,255};
	t_button3.m_colorhover={10,200,100,255};
	t_button3.m_colorclick={100,20,200,255};

	t_button3.Copy(s_button);
    s_button.m_Top=300;
    s_button.m_width=200;
	s_button.m_colorborder={200,23,26,255};
	s_button.m_colorfill={200,23,26,255};
	s_button.m_colorhover={10,200,100,255};
	s_button.m_colorclick={100,20,200,255};
	s_button.m_fValue=&val;
	s_button.m_minValue=10.0f;
	s_button.m_maxValue=50.0f;
	c_panel.Append(&t_button);
	c_panel.Append(&t_button2);
	c_panel.Append(&t_button3);
	c_panel.Append(&s_button);
    newbimg.Init(m_window->m_render ,"Art/button.png", 300, 150);
	cn.Append(&c_panel);
	cn.Append(&c_panel2);
	Objs.Append(&newbimg);
}
void Draw()
{
    //t_button.MousePosition(MouseX, MouseY);
	SDL_SetRenderDrawColor( m_window->m_render, 0x00, 0x00, 0x00, 0xFF );
	SDL_RenderClear(m_window->m_render);
//printf("val %f \n",val);
	//DrawBox(m_window->m_render, 100, 50, 200, 150);
	//new_img.Draw(30, 50,50, 50,0.0, m_window->m_render);
	cn.c_Draw(m_window->m_render);
	//Objs.DrawAll(m_window->m_render);
	SDL_RenderPresent(m_window->m_render);
	cn.c_OnMousePos(MouseX, MouseY, more);

}
int main(int argc, char* args[])
{
	SDL_Event ev;
	Init();


	unsigned int starttime;
	unsigned int currenttime;

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
			/*if(ev.type == SDL_MOUSEBUTTONUP)
			{
				more=false;
			 }*/
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
	more=false;
	if(1000/FPS > currenttime- starttime)
	{
		SDL_Delay(1000/FPS-(currenttime-starttime));
	}
	}
	delete m_window;
	return 0;
}

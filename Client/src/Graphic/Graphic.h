/*
 * Graphic.h
 *
 *  Created on: 22.11.2014 ã.
 *      Author: Niel-Home
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_
#include "Vector2D.h"
#include <SDL_image.h>
#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include<string>
#include <fstream>

using namespace std;

class Picture
{
public:
	Picture();
	~Picture();
	void Init(SDL_Renderer* render, std::string source);
	void Draw(int x, int y,int width, int height,float angle, SDL_Renderer* render);
    std::string Read_Data(std::string delimiter, std::string prefix);

	ifstream m_read_file;
	string m_source;
	int m_width;
	int m_height;
	int m_posx;
	int m_posy;
	float m_angle;
	Vector2D m_center;
	SDL_Surface* m_file;
	SDL_Texture* m_image;
	SDL_RendererFlip m_flip;
};

std::string delSpace(std::string &tmp_str);
std::string deCaps(std::string tmp);

int To_int(string tmp);
void DrawBox(SDL_Renderer* render, int x, int y, int width, int height,SDL_Color& boxcolor);
void FillBox(SDL_Renderer* render, int x, int y, int width, int height,SDL_Color& fillcolor);
void DrawCircle(SDL_Renderer* render, int center_x, int center_y, int radius,SDL_Color& circlecolor);

#endif /* GRAPHIC_H_ */

/*
 * Window.h
 *
 *  Created on: 22.11.2014 ã.
 *      Author: Niel-Home
 */

#ifndef WINDOW_H_
#define WINDOW_H_
#include "SDL.h"
#include "Definitions.h"
#include "string"

using namespace std;

class Window
{
public:
	Window();
	~Window();
	bool InitWindow();

	SDL_Window* m_Window;
	SDL_Renderer* m_render;
};
#endif /* WINDOW_H_ */

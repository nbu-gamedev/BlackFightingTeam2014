/*
 * Elements.h
 *
 *  Created on: 25.11.2014 ã.
 *      Author: Niel-Home
 */

#ifndef ELEMENTS_H_
#define ELEMENTS_H_
#include"stdio.h"
#include"stdlib.h"
#include "Graphic.h"
#include "vector"

class Container
{
public:
	 int m_posx, m_posy;
	 int m_width, m_height;
	 bool m_active, m_visible;
	 long ColorBorder[4];
	 long ColorFill[4];
	 Container* m_owner;
	 std::vector<Container *> m_member;
	 Container(){
	 		 m_posx=0;
	 		 m_posy=0;
	 		 m_width=0;
	 		 m_height=0;
	 		 m_active=false;
	 		 m_visible=true;
	 		 m_owner=NULL;
	 }
	 virtual ~Container()
	 {

	 }


	 void Init(int t_x, int t_y, int width, int height){
		 m_posx=t_x;
		 m_posy=t_y;
		 m_width=width;
		 m_height=height;
		 printf("x=%i\n", m_posx);
		 printf("y=%i\n", m_posy);
		 printf("w=%i\n", m_width);
		 printf("h=%i\n", m_height);
	 };
	 void Append(Container* new_member)
	 {
		 new_member->m_owner=this;
		 m_member.push_back(new_member);
	 }
	 void c_Draw(SDL_Renderer* c_render)
	 {
		 if(!m_member.empty())
		 {
			 for(unsigned int i = 0; i<m_member.size(); i++)
			 {
				 m_member[i]->Draw(c_render);
			 }
		 }
	 }
protected:
	 virtual void Draw(SDL_Renderer* render){};
	 virtual void MousePosition(){}

};

class Panel: public Container
{
public:
	Panel(){
		 m_posx=0;
		 m_posy=0;
		m_width=0;
		m_height=0;

	}
	 virtual void Draw(SDL_Renderer* render)
	 {
		 //printf("I am drawing\n");
		 DrawBox(render, m_posx, m_posy, m_width, m_height, ColorFill[0], ColorFill[1], ColorFill[2], ColorFill[3]);
		 FillBox(render, m_posx, m_posy, m_width, m_height, ColorFill[0], ColorFill[1], ColorFill[2], ColorFill[3]);
        printf("x=%i\n", m_posx);
		 printf("y=%i\n", m_posy);
		 printf("w=%i\n", m_width);
		 printf("h=%i\n", m_height);
	 }
};
#endif /* ELEMENTS_H_ */

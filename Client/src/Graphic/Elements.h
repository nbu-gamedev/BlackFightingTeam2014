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
#include "algorithm"
#include "string"
struct Color
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
};
enum Align
{
    Aline_center,
    Aline_left,
    Aline_right,
    Aline_normal,
};
enum DrawType//choose which of the both drawing type to use the object. main value-dColor
{
    dPicture,
    dColor,
};
enum ElementWidth// choose the width of th object: to take the parent width or to use his own value for width
{
    ParentWidth,
    OwnWidth,
};

class Container
{
public:
	 int m_Left, m_Top;
	 int m_width, m_height;
	 bool m_active, m_visible;
	 Container* m_owner;
	 int numbre;
	 int x=0;//change the name of the variable
	 int y=0;//change the name of the variable
	 bool m_Clicked=false;
	 int m_Aline;
	 int m_custamWidth;
	 int m_DrawType;
	 float m_angle;
	 ButtonImg m_image;
	 std::vector<Container *> m_member;
	 Container():m_Left(0),m_Top(0),m_width(0),m_height(0),m_active(true),m_visible(true),m_owner(NULL), m_Aline(Aline_normal), m_custamWidth(OwnWidth),m_DrawType(dColor), m_angle(0.0f) {
	 		 numbre=0;
	 }
	 virtual ~Container()
	 {
        for(unsigned int i=0; i<m_member.size(); i++)
        {
            delete m_member[i];
            m_member[i]=NULL;
            m_member.erase(m_member.begin()+i);
        }
        m_member.clear();
        delete m_owner;
	 }
void ClientToScreen(int &x, int &y) const//finding the position of the object toward their parents's position
	{

		x += m_Left;
		y += m_Top;

		if( m_owner ){
            if((m_Left+m_width)>((m_owner->m_Left)+(m_owner->m_width)))
            {
                x=((m_owner->m_Left)+(m_owner->m_width)-(m_width)+(m_owner->m_Left));
            }
            if((m_Top+m_height)>((m_owner->m_Top)+(m_owner->m_height)))
            {
                y=((m_owner->m_Top)+(m_owner->m_height))-(m_height+(m_owner->m_Top));
            }
			m_owner->ClientToScreen(x, y);
		}
    }

//Append new element to the parent element
	 void Append(Container* new_member)
	 {
		 new_member->m_owner=this;
		 new_member->CustomizeObjPos();
		 m_member.push_back(new_member);
		std::sort(m_member.begin(), m_member.end());
	 }

//Draw elements to the screen
    void c_Draw(SDL_Renderer* render)
    {
            if(!m_visible)
                return;
           x=0;
           y=0;


            //c_CustomizeObjPos();
            ClientToScreen(x, y);
            Draw(render, x, y);
			 for(auto it = m_member.begin(); it != m_member.end(); it++)
			 {
			     //printf("TOP:%i\n", y);
				 (*it)->c_Draw(render);
			 }
    }

//find positions to the objects :align and width
    void c_CustomizeObjPos()
    {
            if(!m_visible)
                return;
           x=0;
           y=0;
            //

			 for(auto it = m_member.begin(); it != m_member.end(); it++)
			 {
				 (*it)->c_CustomizeObjPos();
			 }
    }

//mouse position to the screen
    void c_OnMousePos(int mousex, int mousey, bool lbutton)
    {
        if(!m_visible)
                return;
        MousePosition(mousex, mousey, lbutton);
        for(auto it = m_member.begin(); it!=m_member.end(); it++)
        {
            (*it)->c_OnMousePos(mousex, mousey, lbutton);
        }
    }

    virtual void Init(int tmp_Left, int tmp_Top, int width, int height, SDL_Renderer* tmp_render, int tmp_Aline=Aline_normal, int tmp_Width=OwnWidth, int tmp_DrawType=dColor, std::string tmp_source="", int tmp_angle=0.0f ){
        m_Left=tmp_Left;
        m_Top=tmp_Top;
        m_width=width;
        m_height=height;
        m_Aline=tmp_Aline;
        m_custamWidth=tmp_Width;
        m_DrawType=tmp_DrawType;
        m_angle=tmp_angle;
        if(m_DrawType==dPicture)
        {
            m_image.Init(tmp_render, tmp_source, tmp_Left, tmp_Top);
            m_image.m_img_width=width;
            m_image.m_img_height=height;
        }
    }

protected:
	 virtual void Draw(SDL_Renderer* render, int x, int y){}
	 virtual void MousePosition(int mousex, int mousey, bool lbutton){}
	 virtual void CustomizeObjPos(){
	 switch(m_Aline)
        {
        case Aline_left:
            m_Left=0;
            break;
        case Aline_right:
            m_Left=(((m_owner->m_width))-m_width);
            break;
        case Aline_center:
            m_Left=((m_owner->m_width)-m_width)/2;
            break;
        case Aline_normal:
            m_Left=m_Left;
            break;
        }

        switch(m_custamWidth)
        {
        case ParentWidth:
            {
                m_width=m_owner->m_width;
                m_Left=0;
                m_Top=m_Top;
                break;
            }
        case OwnWidth:
            {
                m_width=m_width;
                break;
            }

        }
	 }


};



class Panel: public Container
{
public:
    SDL_Color m_colorborder, m_colorfill;
	Panel()
	{
        m_colorborder={255,255,255,255};
        m_colorfill={255,255,255,255};
	}
	~Panel()
	 {
        for(unsigned int i=0; i<m_member.size(); i++)
        {
            delete m_member[i];
            m_member[i]=NULL;
            m_member.erase(m_member.begin()+i);
        }
        delete m_owner;
	 }
	void Copy(Panel& other) const
	{
		other.m_Left = m_Left;
		other.m_Top = m_Top;
		other.m_width = m_width;
		other.m_height = m_height;
		other.m_Aline=m_Aline;
        other.m_custamWidth=m_custamWidth;
        other.m_colorborder=m_colorborder;
        other.m_colorfill=m_colorfill;
	}

protected:
	/* virtual void CustomizeObjectPos()
	 {
	     switch(m_Aline)
        {
        case Aline_left:
            m_Left=0;
            break;
        case Aline_right:
            m_Left=(((m_owner->m_Left)+(m_owner->m_width))-m_width);
            break;
        case Aline_center:
            m_Left=((m_owner->m_width)-m_width)/2;
            break;
        case Aline_normal:
            m_Left=m_Left;
            break;
        }

        switch(m_custamWidth)
        {
        case ParentWidth:
            {
                m_width=m_owner->m_width;
                m_Left=0;
                break;
            }
        case OwnWidth:
            {
                m_width=m_width;
                break;
            }

        }
	 }*/
	 virtual void Draw(SDL_Renderer* render, int x, int y)
	 {
	    // CustomizeObjectPos();
	    if(m_DrawType==dPicture)
        {
            m_image.Update(x, y, m_angle,0);
            m_image.Draw(render);
        }
    else{
		 DrawBox(render, x, y, m_width, m_height, m_colorborder);
		 FillBox(render, x, y, m_width, m_height, m_colorfill);
		 }
	 }
};




class Button:public Panel{
public:
    bool m_onHover;
    bool m_Clicked;
    SDL_Event m_event;
    SDL_Color m_colorhover,m_colorclick;
    Button():m_Clicked(false)
    {
        m_colorhover={255,255,255,255};
        m_colorclick={0,255,255,255};
    }
    void (*m_pOnClick)();

    void Copy(Button& other) const
	{
		other.m_Left = m_Left;
		other.m_Top = m_Top;
		other.m_width = m_width;
		other.m_height = m_height;
		other.m_Aline=m_Aline;
        other.m_custamWidth=m_custamWidth;
        other.m_colorborder=m_colorborder;
        other.m_colorfill=m_colorfill;
        other.m_colorhover=m_colorhover;
        other.m_colorclick=m_colorclick;
	}
protected:
    virtual void OnClick(){ if(m_pOnClick) m_pOnClick();}

    virtual void Draw(SDL_Renderer* render, int x, int y)
    {
        //m_Aline=Aline_normal;
        //m_custamWidth=OwnWidth;
        //CustomizeObjectPos();
        if(m_DrawType==dPicture)
        {
            m_image.Draw(render);
        }
    else{
        DrawBox(render, x,y, m_width, m_height, m_colorborder);
		 FillBox(render,x, y, m_width, m_height, (m_onHover)?m_colorhover:(m_Clicked)?m_colorclick:m_colorfill);
        }
    }

    virtual void MousePosition(int mousex, int mousey,bool  lbutton){
        if((x<=mousex)&&((x+m_width)>=mousex)&&(y<=mousey)&&((y+m_height)>=mousey))
        {
            m_onHover=true;
            //printf("mouse x:%i\n", (mousex-(m_owner->m_Left)+m_Left));
            if(lbutton){
                OnClick();
                m_Clicked=!m_Clicked;


            }



        }
        else{
            m_onHover=false;
            }

        if(m_DrawType==dPicture)
            {
                if(m_onHover)
                {
                    m_image.Update(x, y, m_angle, 1);
                }
                else if(m_Clicked)
                {
                    m_image.Update(x, y, m_angle, 2);
                }
                else{
                    m_image.Update(x, y, m_angle, 0);
                }

            }
    }
};
class Slider:public Button//SliderButton Still not counting the true coordinate of the mouse
{
        public:
            SDL_Color m_colorslider;
            int m_proc;
            Slider():m_proc(20){
            m_colorslider={0, 0, 255, 255};
            }

        protected:
            virtual void Draw(SDL_Renderer* render, int x, int y)
            {if(m_DrawType==dPicture)
                {
                   // m_image.Draw(x,y, m_width, m_height,m_angle,render);
                }
            else{
                DrawBox(render, x,y, m_width, m_height, m_colorborder);
                FillBox(render,x, y, m_width, m_height, m_colorfill);
                FillBox(render,x+2, y+2, (m_width*m_proc)/100-2, m_height-4, m_colorslider);
            }
            }

            virtual void MousePosition(int mousex, int mousey,bool  lbutton){
                int mousexchangeVal=(mousex-x);

                 if((x<=mousex)&&((x+m_width)>=mousex)&&(y<=mousey)&&((y+m_height)>=mousey))
                {
                printf("mousex:%i\n",mousexchangeVal );
                if(lbutton){
                    printf("mousex:%i\n",mousexchangeVal );
                    m_proc=float((mousexchangeVal*100)/m_width);
                    if(mousexchangeVal<0)
                    {
                        m_proc=0;
                    }
                    if(mousexchangeVal>100)
                    {
                        m_proc=100;
                    }
                }

                }
            }
};
#endif /* ELEMENTS_H_ */

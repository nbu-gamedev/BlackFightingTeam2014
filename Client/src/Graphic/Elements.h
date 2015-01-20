/*
 * Elements.h
 *
 *  Created on: 25.11.2014 ã.
 *      Author: Niel-Home
 */
#ifndef ELEMENTS_H_
#define ELEMENTS_H_
#include"stdio.h"
#include "Graphic.h"
#include "vector"
#include "algorithm"
#include "string"
#include <stdlib.h>
#include <iostream>
#include "Definitions.h"
int To_int(std::string tmp);

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
	~Container()
	 {

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
	 virtual void readFile(std::string filename){}
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
    std::vector<PanelInfo> m_PanelInfo;
    SDL_Color m_colorborder, m_colorfill;
	Panel()
	{
        m_colorborder={255,255,255,255};
        m_colorfill={255,255,255,255};

	}
	~Panel()
	 {

	 }

	void Copy(Panel& other) const
	{
		other.m_Left = m_Left;
		other.m_Top = m_Top;
		other.m_width = m_width/2;
		other.m_height = m_height/2;
		other.m_Aline=m_Aline;
        other.m_custamWidth=m_custamWidth;
        other.m_colorborder=m_colorborder;
        other.m_colorfill=m_colorfill;
	}
    virtual void readFile(std::string filename){
        std::ifstream file;
        file.open(filename);
        bool firstline=true;
        unsigned int strbeg=0;
        std::vector<std::string>tmpVars;
        if(file.is_open())
        {
            while(!file.eof())
            {
                std::string tmp="";
                if(firstline){
                    getline(file, tmp);
                    firstline=false;
                }
                else{
                        getline(file, tmp);
                        tmpVars.clear();
                        for(unsigned int i = 0; i<=tmp.size(); i++)
                            {
                                if((tmp[i]==','))
                                {
                                    tmpVars.push_back(tmp.substr(strbeg, i-strbeg));
                                    strbeg=i+1;
                                }
                                if(i==tmp.size())
                                {
                                     tmpVars.push_back(tmp.substr(strbeg, i-strbeg));
                                }
                            }
                            strbeg=0;
                            PanelInfo info;
                             info.width=To_int(tmpVars[0]);
                             info.height=To_int(tmpVars[1]);
                             info.left=To_int(tmpVars[2]);
                             info.top=To_int(tmpVars[3]);
                            m_PanelInfo.push_back(info);
                             std::cout<<info.width<<"\n";
                            std::cout<<info.height<<"\n";
                            std::cout<<info.left<<"\n";
                            std::cout<<info.top<<"\n";


                }


            }

         }
         for(unsigned int i =0; i<m_PanelInfo.size()-1; i++)
         {
             std::cout<<m_PanelInfo[i].width<<"\n";
                            std::cout<<m_PanelInfo[i].height<<"\n";
                            std::cout<<m_PanelInfo[i].left<<"\n";
                            std::cout<<m_PanelInfo[i].top<<"\n";
         }
	 }
protected:

	 virtual void Draw(SDL_Renderer* render, int x, int y)
	 {
	    // CustomizeObjectPos();
	    if(m_DrawType==dPicture)
        {
           // m_image.Update(x, y, m_angle,0);
           //printf("m_image height %i\n",  m_image.m_img_height);
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
    ~Button()
	 {

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
    float *m_fValue;
    float m_minValue, m_maxValue;
    SDL_Color m_colorslider;
    int m_proc;
    Slider():m_proc(20),m_minValue(0), m_maxValue(0){
        m_colorslider={0, 0, 255, 255};
        //*m_fValue=0;
        }
   ~Slider()
	 {

	 }
        protected:
            virtual void Draw(SDL_Renderer* render, int x, int y)
            {if(m_DrawType==dPicture)
                {
                   //m_image.Draw(x,y, m_width, m_height,m_angle,render);
                }
            else{
                m_proc=((*m_fValue*100)/m_maxValue);
                DrawBox(render, x,y, m_width, m_height, m_colorborder);
                FillBox(render,x, y, m_width, m_height, m_colorfill);
                 FillBox(render,x+2, y+2, ((m_width*m_proc)/100)-4, m_height-4, m_colorslider);
            }
            }

            virtual void MousePosition(int mousex, int mousey,bool  lbutton){
                int mousexchangeVal=(mousex-x);

                 if((x<=mousex)&&((x+m_width)>=mousex)&&(y<=mousey)&&((y+m_height)>=mousey))
                {
                printf("mousex:%i\n",mousexchangeVal );
                if(lbutton){
                m_Clicked=!m_Clicked;


                }

                    if(m_Clicked){
                   // printf("mousex:%i\n",mousexchangeVal );
                    m_proc=float((mousexchangeVal*100)/m_width);
                    if(mousexchangeVal<0)
                    {
                        m_proc=0;
                    }

                    *m_fValue=(m_proc*m_maxValue)/100;
                    if(*m_fValue<m_minValue)
                    {
                        *m_fValue=m_minValue;
                    }
                }

                }
                else{
                    m_Clicked=false;
                }
            }
};
#endif /* ELEMENTS_H_ */

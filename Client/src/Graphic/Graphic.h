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
#include <vector>
#include <list>

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

int To_int(string tmp);
void DrawBox(SDL_Renderer* render, int x, int y, int width, int height,SDL_Color& boxcolor);
void FillBox(SDL_Renderer* render, int x, int y, int width, int height,SDL_Color& fillcolor);
void DrawCircle(SDL_Renderer* render, int center_x, int center_y, int radius,SDL_Color& circlecolor);

class DrawObjects
{
public:
  DrawObjects(){}
  ~DrawObjects(){}
  std::list<DrawObjects*> AllObjects;
  std::list<DrawObjects*>::iterator it;
  void DrawAll(SDL_Renderer* render)
  {
      for(it=AllObjects.begin();it!=AllObjects.end();++it)
      {
          (*it)->Draw(render);
      }
  }
  void Append(DrawObjects* member)
  {
      AllObjects.push_back(member);
  }
public:

  virtual void Draw(SDL_Renderer* render){}
};

class ButtonImg: public DrawObjects
{
public:
    float m_width,m_height;
    float m_frames_x;
    float m_frames_y;
    float m_frame_duration;
    unsigned int m_startTime;
    unsigned int m_currentTime;
    float m_angle;
    int m_img_width;
    int m_img_height;
    int m_row_elements;
    int m_column_num;
    std::vector<SDL_Rect>m_vector_frames;
    int m_vector_index;
    int m_xCoordinate, m_yCoordinate;

    SDL_Surface* m_file;
    SDL_Texture* m_sprite;
    SDL_RendererFlip m_flip;

    ButtonImg();
    ~ButtonImg();
    void Init(SDL_Renderer* render, string source, int x=10, int y=100);
    void Update(int x, int y, float angle,int indexOfelement);
public:
    virtual void Draw(SDL_Renderer* render){
    SDL_Rect Destination{m_xCoordinate, m_yCoordinate, m_img_width, m_img_height};
    SDL_RenderCopyEx(render,m_sprite,&m_vector_frames[m_vector_index],&Destination,m_angle,NULL,m_flip);

}

};
/*class Animation:public DrawObjects
{
public:

    ifstream m_read_file;

    float m_width,m_height;

    string m_source;
    float m_frames_x;
    float m_frames_y;
    float m_frame_duration;
    int m_repeat_from_index;
    int m_position;
    string m_animation_type;
    string m_flip_type;
    int m_repetition_searching_var;
    unsigned int m_startTime;
    unsigned int m_currentTime;

    int m_img_width;
    int m_img_height;
    int m_rand_index;
    int m_percentage_height;
    int m_prev_percentage;
    int m_angle;




    int m_row_elements;
    int m_column_num;


    bool m_return;
    bool m_more;

    std::vector<SDL_Rect>m_vector_frames;
    int m_vector_index;
    int m_prev_vector_index;

    SDL_Surface* m_file;
    SDL_Texture* m_sprite;
    SDL_RendererFlip m_flip;



    Animation();
    ~Animation();


    string Read_Data(string delimiter, string prefix);
    void Init(SDL_Renderer* render, string source);


    void Draw_Image(int x, int y, int angle, bool more, SDL_Renderer* render);

    void(*m_TypeDraw)(int,int,int,bool,SDL_Renderer*)  =NULL;
    void Draw(int x, int y, int angle, bool more, SDL_Renderer* render);

};
*/
#endif /* GRAPHIC_H_ */

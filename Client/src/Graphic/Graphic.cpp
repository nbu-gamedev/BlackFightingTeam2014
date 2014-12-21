#include"Graphic.h"


string delSpace(string &tmp_str)
{
    while(' ' == tmp_str[0] && 0<tmp_str.length()) tmp_str.erase(tmp_str.begin());
    while(' ' == tmp_str[tmp_str.length()-1] && 0<tmp_str.length()) tmp_str.erase(tmp_str.end()-1);

    return tmp_str;
}

string deCaps(string tmp)
{
    for(unsigned int i=0; i<tmp.length(); i++)
    {
        if((tmp[i]>='A')&&(tmp[i]<='Z'))
        {
            tmp[i]=(tolower(tmp[i]));
        }
    }
    return tmp;
}

int To_int(string tmp)
{
    int numbre=0;
    if ('0' <= tmp[0] && tmp[0] <= '9')
        {
            numbre=atoi(tmp.c_str());
            tmp="";
        }
    return numbre;
}

Picture::Picture()
{
	m_width=0;
	m_height=0;
	m_posx=0;
	m_posy=0;
	m_angle=0;
	m_source="";
	m_flip=SDL_FLIP_NONE;

}

Picture::~Picture()
{
    SDL_DestroyTexture(m_image);
}

string Picture::Read_Data(string delimiter, string prefix )
{
    bool more=true;
    string tmp;
    string data;

    while(((getline(m_read_file, tmp))||(true==more)))
    {

        if(m_read_file.eof())
        {
            more = false;
            m_read_file.clear();
            m_read_file.seekg(0, ios_base::beg);
            getline(m_read_file, tmp);
        }

        data=deCaps(tmp.substr(0, tmp.find(delimiter)));
        data=delSpace(data);
        prefix=deCaps(prefix);

        if(tmp[0]=='/')
        {

        }
        else
            if(data==prefix)
            {
                data=tmp.substr((tmp.find(delimiter)+1), tmp.size());
                data=delSpace(data);
                tmp="";
                return data;
            }
    }

    if((false==more) && m_read_file.eof())
    {
        data = "none";
    }

    return data;
}

void Picture::Init(SDL_Renderer* render, string source)
{



	    if(m_image != NULL)
	    {
	        SDL_DestroyTexture(m_image);
	        printf("Error Creating new texture : %s\n", SDL_GetError());
	    }
	    m_file = IMG_Load(source.c_str());
	        if(m_file == NULL)
	        {
	            printf("ERROR opening image : %s , SDL_Eror: %s\n",(char*)source.c_str(),SDL_GetError());
	        }
	        else
	        {
	            m_image = SDL_CreateTextureFromSurface(render,m_file);
	            if(m_image == NULL)
	            {
	                printf("ERROR Creating Texture from : %s , SDL_Eror: %s\n",(char*)m_source.c_str(),SDL_GetError());
	            }
	            else
	            {
	                m_width = m_file->w;
	                m_height = m_file->h;

	            }
	            SDL_FreeSurface(m_file);
	        }
}

void Picture::Draw(int x, int y,int width, int height, float angle, SDL_Renderer* render){
	SDL_Rect Destination{x, y, width, height};
	SDL_RenderCopyEx(render,m_image,NULL,&Destination, angle,NULL, m_flip);
}


void DrawBox(SDL_Renderer* render, int x, int y, int width, int height,SDL_Color& boxcolor)
{


	SDL_SetRenderDrawColor(render,boxcolor.r,boxcolor.g,boxcolor.b,boxcolor.a  );

	SDL_Rect Rectagle{x, y, width, height};
	SDL_RenderDrawRect(render,&Rectagle);
}

void FillBox(SDL_Renderer* render, int x, int y, int width, int height,SDL_Color& fillcolor)
{
	SDL_SetRenderDrawColor(render, fillcolor.r,fillcolor.g,fillcolor.b,fillcolor.a  );
	SDL_Rect Rectagle{x, y, width, height};
	SDL_RenderFillRect( render, &Rectagle);
}

void DrawCircle(SDL_Renderer* render, int center_x, int center_y, int radius,SDL_Color& circlecolor)
{
	float two_pi=6.283f;

	SDL_SetRenderDrawColor(render, circlecolor.r,circlecolor.g,circlecolor.b,circlecolor.a   );
	float angle_inc=1.0f/radius;
	for(float angle=0.0f; angle<= two_pi;angle+=angle_inc){
	    int xpos=center_x+radius*cos(angle);
	    int ypos=center_y+radius*sin(angle);
	    SDL_RenderDrawPoint(render, xpos, ypos);
	}
}
ButtonImg::ButtonImg()
{
    m_frames_x=0.0f;
    m_frames_y=0.0f;
    m_frame_duration=0.0f;
    m_startTime=0;
    m_currentTime=0;
    m_angle=0.0f;
    m_img_width=0;
    m_img_height=0;
    m_xCoordinate=0;
    m_yCoordinate=0;

    m_vector_index=0;
    m_file = NULL;
    m_sprite = NULL;
}
ButtonImg::~ButtonImg()
{

}
void ButtonImg::Init(SDL_Renderer* render, string source, int x, int y)
{
     m_file = IMG_Load(source.c_str());

    if(m_sprite != NULL)
    {
        SDL_DestroyTexture(m_sprite);
    }
        if(m_file == NULL)
        {
            printf("ERROR opening image : %s , SDL_Eror: %s\n",(char*)source.c_str(),SDL_GetError());
        }
        else
        {
            m_sprite = SDL_CreateTextureFromSurface(render,m_file);
            if(m_sprite == NULL)
            {
                printf("ERROR Creating Texture from : %s , SDL_Eror: %s\n",(char*)source.c_str(),SDL_GetError());
            }
            else
            {
                m_width = m_file->w;
                m_height = m_file->h;
            }
            SDL_FreeSurface(m_file);
        }

        m_frames_x=1;
        m_frames_y=3;
        SDL_Rect Frames;
        Frames.x=0;
        Frames.y=0;
        Frames.w=int(m_width/m_frames_x);
        Frames.h=int(m_height/m_frames_y);

printf("Frames.w- %i \n", Frames.w);
printf("Frames.h- %i \n", Frames.h);


        m_vector_index=0;
        m_startTime=SDL_GetTicks();

        for(Frames.y =0; Frames.y<m_height; Frames.y=Frames.y+m_height/m_frames_y)
        {
            for(Frames.x=0; Frames.x<m_width; Frames.x=Frames.x+m_width/m_frames_x)
            {
                m_vector_frames.push_back(Frames);
            }
        }
        if(!m_vector_frames.empty())
        {
            printf("Vector is not empty- size %i\n", m_vector_frames.size());
        }
        m_xCoordinate=x;
    m_yCoordinate=y;
    m_img_width=100; m_img_height=100;
    printf("init succ image");
}
void ButtonImg::Update(int x, int y, float angle, int indexOfelement)
{
    m_xCoordinate=x;
    m_yCoordinate=y;
    m_angle=int(m_angle+angle)%360;
    m_vector_index=indexOfelement;
}

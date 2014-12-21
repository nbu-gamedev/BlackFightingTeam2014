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
/*Animation::Animation(){
    m_width = 0;
    m_height = 0;
    m_frames_x = 0;
    m_frames_y = 0;
    m_position=0;
    m_startTime = 0;
    m_currentTime = 0;
    m_frame_duration = 0;
    m_img_height=0;
    m_img_width=0;
    m_rand_index=0;
    m_percentage_height=0;
    m_prev_percentage=0;
    m_vector_index=0;
    m_repeat_from_index=0;
    m_repetition_searching_var=0;
    m_row_elements = 0;
    m_column_num = 0;
    m_angle=0;

    m_source = "";
    m_flip_type="";
    m_animation_type = "";

    m_return=false;
    m_more=true;

    m_flip = SDL_FLIP_NONE;

    m_file = NULL;
    m_sprite = NULL;
}

Animation::~Animation()
{
    SDL_DestroyTexture(m_sprite);
    printf("Destroyed Animation Objects!!!\n");
}

string Animation::Read_Data(string delimiter, string prefix)
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



void Animation::Init(SDL_Renderer* render, string source)
{
    string delimiter = ":";
    m_read_file.open(source);
    if(m_read_file.is_open())
    {
        m_source = Read_Data(delimiter, "file");
        m_frames_x = To_int(Read_Data(delimiter, "frames x"));
        m_frames_y = To_int(Read_Data(delimiter, "frames y"));
        m_frame_duration = To_int(Read_Data(delimiter, "frame duration"));
        m_animation_type = Read_Data(delimiter, "animation type");
        m_flip_type=Read_Data(delimiter, "flip");
        m_img_width=To_int(Read_Data(delimiter, "width"));
        m_img_height=To_int(Read_Data(delimiter, "height"));

        if((m_animation_type=="hpship")||(m_animation_type=="hpbarbody"))
        {
            m_position=To_int(Read_Data(delimiter, "position"));
        }
        else
        {
            m_position=0;
        }
        if(m_animation_type=="repeat middle")
        {
            m_repeat_from_index=To_int(Read_Data(delimiter, "repeat from"));
        }
        else
        {
            m_repeat_from_index=int(m_frames_x/2);
        }
        m_read_file.close();
    }
    else
    {
        m_source="media/Magenta.png";
        m_frames_x=1;
        m_frames_y=1;
        m_frame_duration=0;
        m_animation_type="";
        m_flip_type="none";
    }

    if("horizontal"==m_flip_type)
        {
            m_flip=SDL_FLIP_HORIZONTAL;
        }
        else
            if("vertical"==m_flip_type)
            {
                m_flip=SDL_FLIP_VERTICAL;
            }
            else
                m_flip=SDL_FLIP_NONE;

    if(m_animation_type=="rand")
        {
            srand (time(NULL));
        }

    m_file = IMG_Load(m_source.c_str());

    if(m_sprite != NULL)
    {
        SDL_DestroyTexture(m_sprite);
    }
        if(m_file == NULL)
        {
            printf("ERROR opening image : %s , SDL_Eror: %s\n",(char*)m_source.c_str(),SDL_GetError());
        }
        else
        {
            m_sprite = SDL_CreateTextureFromSurface(render,m_file);
            if(m_sprite == NULL)
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

m_frames_x=1;
m_frames_y=3;
        SDL_Rect Frames;
        Frames.x=0;
        Frames.y=0;
        Frames.w=int(m_width/m_frames_x);
        Frames.h=int(m_height/m_frames_y);

        m_percentage_height=Frames.h;

        m_prev_percentage=100;
        m_vector_index=0;
        m_more=true;
        m_startTime=SDL_GetTicks();

        for(Frames.y =0; Frames.y<m_height; Frames.y=Frames.y+m_height/m_frames_y)
        {
            for(Frames.x=0; Frames.x<m_width; Frames.x=Frames.x+m_width/m_frames_x)
            {
                m_vector_frames.push_back(Frames);
            }
        }
       Animation::m_TypeDraw=Draw_Image;//(0, 0,0.0f, true, render);
}
void Animation::Draw_Image(int x, int y, int angle, bool more, SDL_Renderer* render)
{

    SDL_Rect Destination{x, y, m_img_width, m_img_height};
    SDL_RenderCopyEx(render,m_sprite,&m_vector_frames[0],&Destination,m_angle,NULL,m_flip);
    m_angle=(m_angle+angle)%360;
}
void Animation::Draw(int x, int y, int angle, bool more, SDL_Renderer* render)
{
    //m_TypeDraw(x, y, angle, more,render);
}
*/

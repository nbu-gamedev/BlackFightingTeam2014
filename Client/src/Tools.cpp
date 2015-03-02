#include"Tools.h"
#include <fstream>
#include "SDL_image.h"
#include "SDL_Config.h"

inline bool IS_DIGIT(char a)
{
    if(a >= '0' && a <= '9')
        return true;
    else
        return false;
}

SDL_Rect CenterPicture(SDL_Rect parent, SDL_Rect child)
{
    SDL_Rect tmp;

    tmp.x = ((parent.x + parent.w)/2) - ((child.x+child.w)/2);
    tmp.y = ((parent.y + parent.h)/2) - ((child.y + child.h)/2);
    tmp.w = child.w;
    tmp.h = child.h;
    return tmp;
}

void CreatePicture_Texture(SDL_Texture*& texture, std::string src)
{
    if(texture != NULL)
    {
        SDL_DestroyTexture(texture);
    }
    SDL_Surface* file = NULL;
    file = IMG_Load(src.c_str());
    if(file == NULL)
    {
        std::cout << "ERROR opening image.SDL_Eror: " << SDL_GetError();
    }
    texture = SDL_CreateTextureFromSurface(render,file);
    SDL_FreeSurface(file);
}

std::vector<button> ButtonsList;
std::vector<background>Backgrouds;
std::vector<fonts>Fonts;
std::vector<anime>NinjaAnime;


int StrToInt(std::string data)
{
    int tmp = 0;

    for(unsigned int i = 0;i < data.length();i++)
    {
        tmp *= 10;
        tmp += data[i] - '0';
    }
    return tmp;
}
bool ButtonReadCSV(std::string src)
{
    button tmpB;
    char TMP[1024];
    FILE* file2;
    file2 = fopen(src.c_str(),"r");
    if(file2 != NULL)
    {
        fscanf(file2,"%s",TMP);
        while(fscanf(file2,"%d, %d, %d, %d, %d, %d, %d, %d, %d,%s",&tmpB.index,&tmpB.x1,&tmpB.y1,&tmpB.x2,&tmpB.y2,&tmpB.x3,&tmpB.y3,&tmpB.w,&tmpB.h,TMP)!= EOF)
        {
            tmpB.picture = TMP;
            ButtonsList.push_back(tmpB);
        }
        fclose(file2);
        return true;
    }
    else
    {
        fclose(file2);
        return false;
    }

    return true;
}

//TODO : Fix the reading function.
bool BackGroudsReadCSV(std::string src)
{
    /*background tmpBack;
    FILE* file;
    char TMP[1024];
    file = fopen(src.c_str(),"r");
    if(file != NULL)
    {
        fscanf(file,"%s",TMP);
        while(fscanf(file,"%d,%s",&tmpBack.index,TMP)!=EOF)
        {
            tmpBack.picture = TMP;
            Backgrouds.push_back(tmpBack);
        }
        fclose(file);
        return true;
    }
    else
        {
            std::cout << "Problem with opening the file : " << src << std::endl;
            fclose(file);
            return false;
        }*/

    std::vector<std::string> data;
    std::ifstream file (src.c_str());
    std::string line;
    background tmpBack;
    if(file.is_open())
    {
        while(getline(file,line))
        {
            std::string tmp = "";
            if(IS_DIGIT(line[0]))
            {
                for(unsigned int i = 0;i<line.length()+1;i++)
                {
                    if(line[i] == ',' || line[i] == '\n' || line[i]=='\0')
                    {
                        data.push_back(tmp);
                        tmp = "";
                    }
                    else
                        tmp+=line[i];
                }
                tmpBack.index = StrToInt(data[0]);
                tmpBack.picture = data[1];
                Backgrouds.push_back(tmpBack);
                data.clear();
            }
        }
        file.close();
        return true;
    }
    else
    {
        std::cout << "Problem with opening the file : " << src << std::endl;
        return false;
    }
}

bool FontsReadCSV(std::string src)
{
    std::vector<std::string> data;
    std::ifstream file (src.c_str());
    std::string line;
    fonts tmpfont;

    if(file.is_open())
    {
        while(getline(file,line))
        {
            std::string tmp = "";
            if(IS_DIGIT(line[0]))
            {
                for(unsigned int i = 0;i<line.length()+1;i++)
                {
                    if(line[i] == ',' || line[i] == '\n' || line[i]=='\0')
                    {
                        data.push_back(tmp);
                        tmp = "";
                    }
                    else
                        tmp+=line[i];
                }
                tmpfont.index = StrToInt(data[0]);
                tmpfont.font = data[1];
                Fonts.push_back(tmpfont);
                data.clear();
            }
        }
        file.close();
        return true;
    }
    else
    {
        std::cout << "Problem with opening the file : " << src << std::endl;
        file.close();
        return false;
    }
}


void TestReadCSV(std::string src)
{
    /*anime anim;
    char TMP[1024];
    FILE* file2;
    file2 = fopen(src.c_str(),"r");
    if(file2 != NULL)
    {
        fscanf(file2,"%s",TMP);
        while(fscanf(file2,"%d, %d, %d, %d, %d, %d, %s",&anim.index,&anim.startX,&anim.startY,&anim.endX,&anim.endY,&anim.frames,TMP)!= EOF)
        {
            anim.picture = TMP;
            NinjaAnime.push_back(anim);
        }
    }
    else
    {
    }*/
    std::vector<std::string> data;
    std::ifstream file (src.c_str());
    std::string line;
    anime anim;

    if(file.is_open())
    {
        while(getline(file,line))
        {
            std::string tmp = "";
            if(IS_DIGIT(line[0]))
            {
                for(unsigned int i = 0;i<line.length()+1;i++)
                {
                    if(line[i] == ',' || line[i] == '\n' || line[i]=='\0')
                    {
                        data.push_back(tmp);
                        tmp = "";
                    }
                    else
                        tmp+=line[i];
                }
                anim.index = StrToInt(data[0]);
                anim.startX = StrToInt(data[1]);
                anim.startY = StrToInt(data[2]);
                anim.frames = StrToInt(data[3]);
                anim.w = StrToInt(data[4]);
                anim.h = StrToInt(data[5]);
                anim.picture = data[6];
                NinjaAnime.push_back(anim);
                data.clear();
            }
        }
        file.close();
    }
    else
    {
        std::cout << "Problem with opening the file : " << src << std::endl;
        file.close();
    }
}



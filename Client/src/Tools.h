#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <string>
#include <vector>
#include "Defines.h"

extern std::vector<button> ButtonsList;
extern std::vector<background>Backgrouds;
extern std::vector<fonts>Fonts;
extern std::vector<anime>NinjaAnime;

int StrToInt(std::string data);
bool ButtonReadCSV(std::string src);
bool BackGroudsReadCSV(std::string src);
bool FontsReadCSV(std::string src);
void TestReadCSV(std::string src);


SDL_Rect CenterPicture(SDL_Rect parent, SDL_Rect child);
void CreatePicture_Texture(SDL_Texture*& texture, std::string src);

#endif // TOOLS_H_INCLUDED

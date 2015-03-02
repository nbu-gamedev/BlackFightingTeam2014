#ifndef XML_PARSER_H_INCLUDED
#define XML_PARSER_H_INCLUDED

#include "ExtraLibs/tinyxml2.h"
#include "Graphic.h"
#include "Tools.h"
#include "Game.h"
#include "Elements.h"

using namespace tinyxml2;

class StateParser
{
public:
    bool parseState(const char* stateFile,std::string stateID,std::vector<GameObjects*> *pObjects,std::vector<std::string>*pTextureIDs);
private:
    void parseObjects (tinyxml2::XMLElement* pStateRoot,std::vector<GameObjects*> *pObjects);
    void parseTextures (tinyxml2::XMLElement* pStateRoot,std::vector<std::string> *pTextureIDs);
};

#endif // XML_PARSER_H_INCLUDED

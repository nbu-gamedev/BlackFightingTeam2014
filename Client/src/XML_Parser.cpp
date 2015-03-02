#include "XML_Parser.h"
using namespace tinyxml2;
using namespace std;
bool StateParser::parseState(const char* stateFile,std::string stateID,std::vector<GameObjects*> *pObjects,std::vector<std::string>*pTextureIDs)
{
    XMLDocument doc;

    if(doc.LoadFile(stateFile))
    {
        std::cerr << doc.ErrorName() << std::endl;
        return false;
    }
    XMLElement* pRoot = doc.RootElement();

    for(tinyxml2::XMLElement* e = pRoot->FirstChildElement();e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == stateID)
        {
            pRoot = e;
        }
    }
    XMLElement* pTextureRoot = 0;
    for(XMLElement* e = pRoot->FirstChildElement();e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == std::string("TEXTURES"))
        {
            pTextureRoot = e;
        }
    }
    parseTextures(pTextureRoot,pTextureIDs);
    XMLElement* pObjectRoot = 0;
    for(XMLElement* e = pRoot->FirstChildElement();e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == std::string("OBJECTS"))
        {
            pObjectRoot = e;
        }
    }
    parseObjects(pObjectRoot,pObjects);
    return true;
}

void StateParser::parseTextures(tinyxml2::XMLElement* pStateRoot,std::vector<std::string> *pTextureIDs)
{
     for(XMLElement* e = pStateRoot->FirstChildElement();e != NULL; e = e->NextSiblingElement())
     {
         std::string fileName = e->Attribute("filename");
         std::string id = e->Attribute("ID");
         pTextureIDs->push_back(id);
         GM::Instance()->load(fileName,id);
     }
}

void StateParser::parseObjects (tinyxml2::XMLElement* pStateRoot,std::vector<GameObjects*> *pObjects)
{
    for(XMLElement* e = pStateRoot->FirstChildElement();e != NULL; e = e->NextSiblingElement())
    {
        int x,y,w,h,dH,dW,numFrames,callbackID = 0,animSpeed = 0,aligment = 0,mergeY = 0,mergeX = 0;
        std::string textureID;
        e->QueryAttribute("x",&x);
        e->QueryAttribute("y",&y);
        e->QueryAttribute("width",&w);
        e->QueryAttribute("height",&h);
        e->QueryAttribute("dW",&dW);
        e->QueryAttribute("dH",&dH);
        e->QueryAttribute("numberFrames",&numFrames);
        e->QueryAttribute("callbackID",&callbackID);
        e->QueryAttribute("animationSpeed",&animSpeed);
        e->QueryAttribute("aligment",&aligment);
        e->QueryAttribute("mergeX",&mergeX);
        e->QueryAttribute("mergeY",&mergeY);
        textureID = e->Attribute("textureID");

        GameObjects* pGameObj = GOF::Instance()->create(e->Attribute("type"));
        pGameObj->load(new LoadParam(x,y,w,h,dW,dH,textureID,numFrames,aligment,mergeX,mergeY,callbackID,animSpeed));
        pObjects->push_back(pGameObj);
    }
}






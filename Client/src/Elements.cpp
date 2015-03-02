#include "Elements.h"
#include "SDL_image.h"
#include "Input.h"
#include "Game.h"

GameObjectFactory* GameObjectFactory::sInstance = 0;
Cursor* Cursor::s_Instance = 0;

/*
    Classes for objects in the game
    - coordinates, sizes etc...
    TODO: add more var for animation
    TODO: update the animation functions
*/


void GameObjects::load(const LoadParam* pParam)
{
    int tmpW = G::Instance()->getWindowWidth(),tmpH = G::Instance()->getWindowHeight();
    int tmpX,tmpY;

    if(pParam->getAligment() == A_NONE)
    {
        m_position = Vector2D(pParam->getX(),pParam->getY());
    }
    else if (pParam->getAligment() == A_CENTER)
    {
        tmpX =((tmpW/2) - (pParam->getDisplayW()/2))+((float)pParam->m_mergeX/100)* tmpW ;
        tmpY = pParam->getY() + ((float)pParam->m_mergeY/100)*tmpH;
        m_position = Vector2D(tmpX,tmpY);
    }
    else if (pParam->getAligment() == A_LEFT)
    {
        tmpX = (tmpW - pParam->getDisplayW())+((float)pParam->m_mergeX/100)* tmpW;
        tmpY = pParam->getY() + ((float)pParam->m_mergeY/100)*tmpH;
        m_position = Vector2D(tmpX,tmpY);
    }
    else if(pParam->getAligment() == A_RIGHT)
    {
        tmpX = pParam->getX() + ((float)pParam->m_mergeX/100)*tmpW;
        m_position = Vector2D(tmpX,pParam->getY());
    }
    else if(pParam->getAligment() == A_CENTER_DOWN)
    {
        tmpX =((tmpW/2) - (pParam->getDisplayW()/2))+((float)pParam->m_mergeX/100)* tmpW ;
        tmpY = pParam->getY() + (tmpH-pParam->getDisplayH()) + ((float)pParam->m_mergeY/100)*tmpH;
        m_position = Vector2D(tmpX,tmpH);
    }

    setPercentsX_Y(m_position.getX(),m_position.getY());

    m_w = pParam->getW();
    m_h = pParam->getH();
    m_displayW = pParam->getDisplayW();
    m_displayH = pParam->getDisplayH();
    m_textureID = pParam->getTextureID();

    m_curFrame = 0;
    m_curRow = 1;
    m_numFrames = pParam->getNumFrames();
    delete pParam;
}

void GameObjects::setPercentsX_Y(int x,int y)
{
    m_percentX = ((float)x/G::Instance()->getWindowWidth())*100;
    m_percentY = ((float)y/G::Instance()->getWindowHeight())*100;
}

void GameObjects::Draw()
{
    GM::Instance()->drawFrame(m_textureID,(int)m_position.getX(),(int)m_position.getY(),m_w,m_h,m_displayW,m_displayH,m_curRow,m_curFrame,m_angle,m_alpha);
}

bool GameObjectFactory::registerType(std::string typeID, BaseCreator* pCreator)
{
    std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);

    if(it != m_creators.end())
    {
        delete pCreator;
        return false;
    }
    m_creators[typeID] = pCreator;

    return true;
}

GameObjects* GameObjectFactory::create(std::string typeID)
{
    std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);
    //std::cout << (*it).first << " : ";
    if(it == m_creators.end())
    {
        std::cout << "couldnt not find a type : " << typeID << std::endl;
        return NULL;
    }

    BaseCreator* pCreator = (*it).second;
    return pCreator->createGameObject();
}
/*
    END object classes
*/


/*
    Button class
    TODO: rename MenuButton to Button and change the type name in Game Init function
*/

MenuButton::MenuButton():GameObjects()
{
    m_curRow = MOUSE_OUT;
}

 void MenuButton::load(const LoadParam* pParam)
 {
     GameObjects::load(pParam);
     m_Released = true;
     m_callbackID = pParam->getCallbackID();
 }
void MenuButton::Draw()
{
    GameObjects::Draw();
}
void MenuButton::Update()
{
    Vector2D pMousePos;
    pMousePos = Input::Instance()->getMousePosition();
    if(pMousePos.getX() < (m_position.getX() + m_displayW)
     &&pMousePos.getX() > m_position.getX()
     &&pMousePos.getY() < (m_position.getY() + m_displayH)
     &&pMousePos.getY() > m_position.getY())
    {
        if(Input::Instance()->getMouseButtonState(LEFT) && m_Released)
        {
            m_curRow = CLICKED;
            m_callback();
            m_Released = false;
        }
        else if(!Input::Instance()->getMouseButtonState(LEFT))
        {
            m_Released = true;
            m_curRow = MOUSE_OVER;
        }
    }
    else
    {
        m_curRow = MOUSE_OUT;
    }

}
void MenuButton::Clean()
{
    GM::Instance()->clearTextureFromMap(m_textureID);
}
/*
    END Button class
*/


/*
    Background Class Implementation
*/

Background::Background():GameObjects()
{

}

void Background::load(const LoadParam* pParams)
{
    GameObjects::load(pParams);
}

 void Background::Draw()
 {
     GM::Instance()->draw(m_textureID,0,0,G::Instance()->getWindowWidth(),G::Instance()->getWindowHeight());
 }
 void Background::Clean()
 {
     GM::Instance()->clearTextureFromMap(m_textureID);
 }
/*
    END Background class
*/


/*
    Cursor implementation
*/
Cursor::Cursor():GameObjects()
{

}
void Cursor::Init()
 {
     GM::Instance()->load("data/image/UI/CURSOR.PNG","cor1");

     m_curFrame = C_NONE;
     m_active = true;

     m_w = 32;
     m_h = 32;
     m_displayH = 32;
     m_displayW =32;
 }

void Cursor::Update()
{
    Vector2D tmp;
    tmp = Input::Instance()->getMousePosition();
    m_position.SetX(tmp.getX());
    m_position.setY(tmp.getY());
}

void Cursor::Draw()
{
    if(m_active)
    {
        GM::Instance()->drawFrame("cor1",m_position.getX(),m_position.getY(),32,32,32,32,m_curRow,m_curFrame,0,255);
    }
}
/*
    END Cursor
*/

void AnimationGraphic::Update()
{
    m_curFrame = ((SDL_GetTicks()/1000/m_animSpeed)%m_numFrames);
}

void AnimationGraphic::Draw()
{
    GameObjects::Draw();
}

void TextField::load(const LoadParam* pParam)
{
    GameObjects::load(pParam);

    m_fontSize = 18;
    m_active = false;
    m_Released = true;
    m_textInput = "";
    m_bHide = false;
}

void TextField::Update()
{
    Vector2D pMousePos;
    pMousePos = Input::Instance()->getMousePosition();
    if(Input::Instance()->getMouseButtonState(LEFT)&& m_Released)
    {
        if(pMousePos.getX() < (m_position.getX() + m_displayW)&&pMousePos.getX() > m_position.getX()&&pMousePos.getY() < (m_position.getY() + m_displayH)
            &&pMousePos.getY() > m_position.getY())
         {
              m_active = true;
              m_Released = false;
              Input::Instance()->StartTextInput();
              Input::Instance()->setTextInput(m_textInput);
         }
         else
         {
             m_active = false;
         }
     }
     else if(!Input::Instance()->getMouseButtonState(LEFT))
    {
        m_Released = true;
    }

     if(m_active)
     {
         m_textInput = Input::Instance()->getTextInput();
     if(m_bHide)
     {
         unsigned int sizeT = m_textInput.length();
         if(sizeT > m_hideText.length())
         {
             for(unsigned int i = 0;i < sizeT-m_hideText.length();i++)
                m_hideText += "*";
         }
         if(sizeT < m_hideText.length())
         {
             m_hideText = m_hideText.substr(0,m_hideText.length()-(m_hideText.length()-sizeT));
         }
     }
     }
}

void TextField::Draw()
{
    SDL_Rect tmp = {(int)m_position.getX(),(int)m_position.getY(),m_displayW,m_displayH};
    SDL_Color colorTmp = (m_active)?RED:WHITE;

    SDL_SetRenderDrawColor(render,colorTmp.r,colorTmp.g,colorTmp.b,colorTmp.a);
    SDL_RenderDrawRect(render,&tmp);
    GM::Instance()->draw("textBack",m_position.getX(),m_position.getY(),m_displayW,m_displayH);
    GM::Instance()->drawText("data/fonts/OptimusPrincepsSemiBold.ttf",(m_bHide)?m_hideText:m_textInput,m_position.getX(),m_position.getY()+(m_fontSize/2),m_displayW,m_displayH,WHITE,m_fontSize);
}

void TextField::Clean()
{
    GM::Instance()->clearTextureFromMap("textBack");
}

/*
    BLACK FITHING TEAM
*/

void CharacterButton::load(const LoadParam* pParams)
{
    GameObjects::load(pParams);

    m_race = "";
    m_name = "";
    m_rank = "";
    m_bfree = true;
    m_Released = true;
    m_active = false;
}

void CharacterButton::setName_Race(std::string name,int race)
{
    m_name = name;
    if(race == ELF)
    {
        m_race = "Elf";
        m_raceID = ELF;
    }
    else if(race == ORC)
    {
        m_race = "Orc";
        m_raceID = ORC;
    }
    m_bfree = false;
}

void CharacterButton::Draw()
{
    SDL_Rect tmp = {(int)m_position.getX(),(int)m_position.getY(),(int)m_displayW,(int)m_displayH};
    SDL_Color colorTmp = (m_active)?RED:WHITE;
    SDL_SetRenderDrawColor(render,colorTmp.r,colorTmp.g,colorTmp.b,colorTmp.a);

    GM::Instance()->draw(m_textureID,m_position.getX(),m_position.getY(),m_displayW,m_displayH);
    if(m_race.length()>0)
    {
         GM::Instance()->draw(m_race,m_position.getX(),m_position.getY()+20,70,60);
    }

    GM::Instance()->drawText("data/fonts/OptimusPrincepsSemiBold.ttf",m_race,m_position.getX()+75,m_position.getY()+20,m_displayW-75,20,RED,20);
    GM::Instance()->drawText("data/fonts/OptimusPrincepsSemiBold.ttf",m_name,m_position.getX()+75,m_position.getY()+50,m_displayW-75,12,WHITE,12);
    SDL_RenderDrawRect(render,&tmp);
}

void CharacterButton::Update()
{
    Vector2D pMousePos;
    pMousePos = Input::Instance()->getMousePosition();
    if(Input::Instance()->getMouseButtonState(LEFT)&& m_Released)
    {
        if(pMousePos.getX() < (m_position.getX() + m_displayW)&&pMousePos.getX() > m_position.getX()&&pMousePos.getY() < (m_position.getY() + m_displayH)
            &&pMousePos.getY() > m_position.getY())
         {
              m_active = !m_active;
              m_Released = false;
         }
     }
     else if(!Input::Instance()->getMouseButtonState(LEFT))
    {
        m_Released = true;
    }
}

void Picture::load(const LoadParam* pParams)
{
    GameObjects::load(pParams);
}

void Picture::Draw()
{
    GM::Instance()->draw(m_textureID,m_position.getX(),m_position.getY(),m_displayW,m_displayH);
}

void Picture::Clean()
{
    GM::Instance()->clearTextureFromMap(m_textureID);
}














#ifndef ELEMENTS_H_INCLUDED
#define ELEMENTS_H_INCLUDED

#include "Defines.h"
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include "Graphic.h"
#include "Vector2D.h"
#include <functional>

enum button_state{MOUSE_OUT = 0, MOUSE_OVER = 1,CLICKED = 2};
enum aligments{A_NONE = 0,A_CENTER = 1,A_LEFT = 2,A_RIGHT = 3,A_CENTER_DOWN = 4,A_CENTER_UP = 5,A_CENTER_LEFT = 6};

class LoadParam
{
public:
    LoadParam(int x,int y,int w,int h,int dW,int dH,std::string textureID,int numFrames,int aligment=A_NONE,int pmergeX=0,int pmergeY = 0,int callbackID = 0,int animSpeed = 0):
        m_x(x),m_y(y),m_w(w),m_h(h),m_displayW(dW),m_displayH(dH),m_textureID(textureID),
        m_numFrames(numFrames),m_aligment(aligment),m_mergeX(pmergeX),m_mergeY(pmergeY),m_callbackID(callbackID),m_animSpeed(animSpeed){}

    int getX()const {return m_x;}
    int getY()const {return m_y;}
    int getW()const {return m_w;}
    int getH()const {return m_h;}
    int getAnimSpeed()const {return m_animSpeed;}
    int getDisplayW()const {return m_displayW;}
    int getDisplayH()const {return m_displayH;}
    int getNumFrames()const {return m_numFrames;}
    int getCallbackID()const {return m_callbackID;}
    std::string getTextureID()const {return m_textureID;}
    int getAligment()const {return m_aligment;}

    int m_mergeX,m_mergeY;
private:
    int m_x;
    int m_y;

    int m_w;
    int m_h;
    int m_displayW;
    int m_displayH;

    int m_numFrames;
    int m_callbackID;
    int m_animSpeed;
    int m_aligment;

    std::string m_textureID;
};

class GameObjects
{
public:
    virtual ~GameObjects(){}
    virtual void Draw();
    virtual void Update() = 0;
    virtual void Clean() = 0;
    virtual void load(const LoadParam* pParams);

    virtual void Collision() = 0;
    virtual std::string type() = 0;

    Vector2D& getPosition(){return m_position;}
    int getWidth(){return m_displayW;}
    int getHeight(){return m_displayH;}
    int getPercentX(){return m_percentX;}
    int getPercentY(){return m_percentY;}

    bool Updating(){return m_bUpdating;}
    bool Dead(){return m_bDead;}
    bool Dying(){return m_bDying;}
    void setUpdating(bool updating){m_bUpdating = updating;}
    void setPercentsX_Y(int x,int y);
protected:
    GameObjects():m_position(0,0),m_velocity(0,0),m_acceleration(0,0),m_w(0),
    m_h(0),m_displayW(0),m_displayH(0),m_curRow(0),m_curFrame(0),m_numFrames(0),
    m_bDead(false),m_bDying(false),m_bUpdating(false),m_angle(0),m_alpha(255){}

    Vector2D m_position;
    Vector2D m_velocity;
    Vector2D m_acceleration;

    int m_w;
    int m_h;
    int m_displayW;
    int m_displayH;

    int m_percentX,m_percentY;

    int m_curRow;
    int m_curFrame;
    int m_numFrames;
    std::string m_textureID;

    bool m_bDead;
    bool m_bDying;
    bool m_bUpdating;

    double m_angle;
    int m_alpha;

};

class BaseCreator
{
public:
    virtual GameObjects* createGameObject() const = 0;
    virtual ~BaseCreator(){};
};

class GameObjectFactory
{
public:
    bool registerType(std::string typeID, BaseCreator* pCreator);
    GameObjects* create(std::string typeID);

    static GameObjectFactory* Instance()
    {
        if(sInstance == 0)
        {
            sInstance = new GameObjectFactory();
        }
        return sInstance;
    }

private:
    static GameObjectFactory* sInstance;
    ~GameObjectFactory(){}
    std::map<std::string, BaseCreator*> m_creators;
};
typedef GameObjectFactory GOF;

class Shooter: public GameObjects
{
public:
    virtual ~Shooter(){}
    virtual void Draw(){};
    virtual void Update(){};
    virtual void Clean(){};
    virtual void load(const LoadParam* pParams);

    virtual void Collision(){}
    virtual std::string type(){ return "Shooter";}

protected:
    Shooter();
    void doDyingAnime();

    int m_bulletFireSpeed;
    int m_bulletCounter;

    int m_moveSpeed;

    int m_dyingTime;
    int m_dyingCounter;
};

class AnimationGraphic: public GameObjects
{
public:
    AnimationGraphic(const LoadParam* pParams,int animspeed)
    {
        GameObjects::load(pParams);
        m_animSpeed = pParams->getAnimSpeed();
    }
    virtual ~AnimationGraphic(){}
    virtual void Draw();
    virtual void Update();
    virtual void Clean(){}
    virtual void load(const LoadParam* pParams){}

    virtual void Collision(){}
    virtual std::string type(){return "Animation";}
private:
   int m_animSpeed;
};

typedef std::function<void(void)> FunctionName;
class MenuButton:public GameObjects
{
public:
    MenuButton();

    virtual void load (const LoadParam* pParam);
    virtual void Draw();
    virtual void Update();
    virtual void Clean();
    virtual void Collision(){};
    virtual std::string type(){return "MenuButton";}

    void setCallBack(FunctionName callback){m_callback = callback;}
    int getCallback() {return m_callbackID;}

private:
    FunctionName m_callback;
    bool m_Released;
    int m_callbackID;
};

class MenuButtonCreator:public BaseCreator
{
    virtual GameObjects* createGameObject()const
    {
        return new MenuButton();
    }
};

class TextField: public GameObjects
{
public:
    TextField():GameObjects(){}
    virtual void Draw();
    virtual void Update();
    virtual void Clean();

    virtual void Collision(){}
    virtual std::string type(){return "TextField";}

    virtual void load(const LoadParam* pParams);

    void SetPassHide(){m_bHide = true;}
    void setFontSize(int sizeFont){m_fontSize = sizeFont;}
    std::string getText(){return m_textInput;}

private:
    bool m_active;
    bool m_Released;
    bool m_bHide;
    std::string m_textInput;
    std::string m_hideText;
    int m_fontSize;
};

class TextFieldCreator:public BaseCreator
{
    virtual GameObjects* createGameObject()const
    {
        return new TextField();
    }
};

class Background:public GameObjects
{
public:
    Background();

    virtual void Draw();
    virtual void Update(){};
    virtual void Clean();

    virtual void Collision(){}
    virtual std::string type(){return "Background";}

    virtual void load(const LoadParam* pParams);
};

class BackGroundCreator:public BaseCreator
{
    virtual GameObjects* createGameObject() const
    {
        return new Background();
    }
};

enum {C_NONE = 0,OVER = 1,C_CLICKED = 2};
class Cursor:public GameObjects
{
public:
    Cursor();

    virtual void Draw();
    virtual void Update();
    virtual void Clean(){};
    virtual void load(const LoadParam* pParams){};

    virtual void Collision(){}
    virtual std::string type(){return "Cursor";}

    void Init();
    void SetClicked(){m_curFrame = C_CLICKED;}
    void SetNone(){m_curFrame = C_NONE;}
    void setActive(){m_active = true;}
    void setUnactive(){m_active = false;}

    static Cursor* Instace()
    {
        if(s_Instance == 0)
        {
            s_Instance = new Cursor();

        }
        return s_Instance;
    }
private:
    static Cursor* s_Instance;
    bool m_active;
};
typedef Cursor TheCoursor;

/*
    BLACKFITHING TEAM STUFF
*/

class Picture:public GameObjects
{
public:
    Picture():GameObjects(){}
    virtual void Draw();
    virtual void Update(){};
    virtual void Clean();

    virtual void Collision(){}
    virtual std::string type(){return "Picture";}

    virtual void load(const LoadParam* pParams);
};

class PictureCreator:public BaseCreator
{
    virtual GameObjects* createGameObject() const
    {
        return new Picture();
    }
};

enum{ELF = 1,ORC = 2};
class CharacterButton:public GameObjects
{
 public:
    CharacterButton():GameObjects(){m_bfree = true;m_active = false;m_Released = true;}
    virtual void Draw();
    virtual void Update();
    virtual void Clean(){};

    virtual void Collision(){}
    virtual std::string type(){return "CharacterButton";}

    virtual void load(const LoadParam* pParams);

    void setName_Race(std::string name,int race);
    bool isActive(){return m_active;}
    bool isFree(){return m_bfree;}
    std::string getName(){return m_name;}
    std::string getRace(){return m_race;}
    unsigned int getRaceID(){return m_raceID;}

 private:
    unsigned int m_raceID;
    bool m_bfree;
    bool m_active;
    bool m_Released;
    std::string m_name;
    std::string m_rank;
    std::string m_race;
};










#endif // ELEMENTS_H_INCLUDED

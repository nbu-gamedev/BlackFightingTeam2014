#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Defines.h"
#include "Graphic.h"
#include "Tools.h"
#include "Elements.h"
#include "StateManager.h"



class Game
{
public:
    ~Game();
    void Init();
    void Draw();
    void Update();
    void handleEvents();
    void Clean();
    void Quit();
    GameStateMachine* getStateMachine(){return m_gameStateMch;}
    int getWindowWidth()const{return m_wWidth;}
    int getWindowHeight()const {return m_wHeight;}

    bool running() {return m_Running;}

    static Game* Instance()
    {
        if(p_Instance == 0)
        {
            p_Instance = new Game();
            return p_Instance;
        }
        return p_Instance;
    }
private:
    Game(){};
    int m_wWidth,m_wHeight;
    bool m_Running;
    static Game* p_Instance;
    GameStateMachine* m_gameStateMch;
};
typedef Game G;

class Player
{
public:
    unsigned int m_race[5]={0,0,0,0};
    std::string m_CharNames[5];
    unsigned int m_indx;
    unsigned int m_ID;
    unsigned int m_HP,m_DMG,m_M_DMG,m_DEF,m_M_DEF;
    bool m_update;
    Player()
    {
        m_HP = 0;
        m_DMG = 0;
        m_M_DMG = 0;
        m_DEF = 0;
        m_M_DEF = 0;
        m_indx = 0;
        m_update = false;
    }
};
extern Player ThePlayer;
#endif // GAME_H_INCLUDED

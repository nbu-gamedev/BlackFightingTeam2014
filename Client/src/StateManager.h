#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED
#include <iostream>
#include <vector>
#include <functional>
#include "Elements.h"
#include "BSDv2.h"

class GameState
{
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual bool Enter() = 0;
    virtual bool Exit() = 0;
    virtual std::string getState() const = 0;

    virtual ~GameState(){}
};

typedef std::function<void(void)> FunctionName;
class MenuState:public GameState
{
protected:
    FunctionName callback;
    virtual void setCallbacks(const std:: vector<FunctionName>& callbacks) = 0;
    std::vector<FunctionName>m_callbacks;
};

class MainMenuState:public MenuState
{
public:
    MainMenuState():MenuState(){}
    virtual void Update();
    virtual void Draw();

    virtual bool Enter();
    virtual bool Exit();
    virtual std::string getState()const {return s_menuID;}
private:
    void s_menuPlay();
    void s_menuExit();
    virtual void setCallbacks(const std:: vector<FunctionName>& callbacks);

    static const std::string s_menuID;
    std::vector<GameObjects*> m_menuObjects;
    std::vector<std::string> m_textureID;
    std::vector<std::string> m_textures;
    TextField* m_loginName;
    TextField* m_password;

    message_t m_server_message;
};

class GameStateMachine
{
public:
    void pushState(GameState* pState);
    void changeState(GameState* pState);
    void popState();

    void Update();
    void Draw();
private:
    std::vector<GameState*> m_gameStates;
};

class ChoseCharState:public MenuState
{
public:
    ChoseCharState():MenuState(){}
    virtual void Update();
    virtual void Draw();

    virtual bool Enter();
    virtual bool Exit();
    virtual std::string getState()const {return s_chosechar;}
private:
    void s_CreateCharWindow();
    void Play();
    void s_Exit();
    virtual void setCallbacks(const std:: vector<FunctionName>& callbacks);

    Picture m_orc;
    Picture m_elf;
    static const std::string s_chosechar;
    std::vector<CharacterButton*> m_characters;
    std::vector<GameObjects*> m_objects;
    std::vector<std::string>m_textures;
    int m_Charactive;
    message_t m_server_message;
};

class PlayState:public MenuState
{
public:
    PlayState():MenuState(){}
    virtual void Update();
    virtual void Draw();

    virtual bool Enter();
    virtual bool Exit();
    virtual std::string getState()const {return s_game;}
    virtual void setCallbacks(const std:: vector<FunctionName>& callbacks);
private:
    void ExitGame();
    static const std::string s_game;
    std::vector<GameObjects*> m_objects;
    std::vector<std::string>m_textures;
};

class CreateChar:public MenuState
{
public:
    CreateChar():MenuState(){}
    virtual void Update();
    virtual void Draw();

    virtual bool Enter();
    virtual bool Exit();
    virtual std::string getState()const {return s_create;}
    virtual void setCallbacks(const std:: vector<FunctionName>& callbacks);
private:
    void s_CreateCharacter();
    void s_Back();
    static const std::string s_create;
    std::vector<CharacterButton*> m_characters;
    std::vector<GameObjects*> m_objects;
    std::vector<std::string>m_textures;
    int m_Picked;
    Picture m_orc;
    Picture m_elf;
    TextField m_CharName;
    message_t m_server_message;
};

#endif // STATEMANAGER_H_INCLUDED

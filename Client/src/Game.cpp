#include "Game.h"
#include "Input.h"

Game* Game::p_Instance = 0;
Player ThePlayer;
void Game::Init()
{
    SDL_ShowCursor(0);
    Cursor::Instace()->Init();
    SDL_GetWindowSize(window,&m_wWidth,&m_wHeight);
    GOF::Instance()->registerType("MenuButton",new MenuButtonCreator());
    GOF::Instance()->registerType("Background",new BackGroundCreator());
    GOF::Instance()->registerType("Picture",new PictureCreator());
    GOF::Instance()->registerType("TextField",new TextFieldCreator());
    GM::Instance()->load("data/image/UI/back.png","textBack");

    m_Running = true;
    m_gameStateMch = new GameStateMachine();
    m_gameStateMch->pushState(new MainMenuState());
}

void Game::Clean()
{
    Input::Instance()->Clean();
    GM::Instance()->Clean();
}

void Game::handleEvents()
{
    Input::Instance()->Update();

    if(Input::Instance()->MouseInScreen())
    {
        Cursor::Instace()->setActive();
        if(Input::Instance()->getMouseButtonState(LEFT))
        {
            Cursor::Instace()->SetClicked();
        }
        else
        {
            Cursor::Instace()->SetNone();
        }
    }
    else
    {
        Cursor::Instace()->setUnactive();
    }

}

void Game::Draw()
{
    m_gameStateMch->Draw();
    Cursor::Instace()->Draw();
}

void Game::Update()
{
    m_gameStateMch->Update();
    Cursor::Instace()->Update();
}

void Game::Quit()
{
    GM::Instance()->Clean();
    m_Running = false;
}

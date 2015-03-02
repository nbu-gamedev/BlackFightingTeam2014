#include "Input.h"

Input* Input::s_Instance = 0;

Input::Input()
{
    for(int i = 0; i < 3;i++)
    {
        m_mouseButtonStates.push_back(false);
    }
    m_text = "";
    StopTextInput();
}

void Input::Clean()
{
    if(m_JoystickInit)
    {
        for( int i = 0; i < SDL_NumJoysticks();i++)
        {
            SDL_JoystickClose(m_joysticks[i]);
        }
    }
}

void Input::InitJoysticks()
{
    if(SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
    {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    if(SDL_NumJoysticks() > 0)
    {
        for(int i = 0;i < SDL_NumJoysticks();i++)
        {
            SDL_Joystick* joy = SDL_JoystickOpen(i);
            if(SDL_JoystickOpen(i))
            {
                m_joysticks.push_back(joy);
            }
            else
            {
                std::cout << SDL_GetError() << std::endl;
            }
        }
        SDL_JoystickEventState(SDL_ENABLE);
        m_JoystickInit = true;
        std::cout << "Initialised " << m_joysticks.size() << " joystick(s).\n";
    }
    else
    {
        m_JoystickInit = false;
    }
}

void Input::Update()
{
    m_keystates = SDL_GetKeyboardState(0);
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            std::cout << "Closing the game\n";
        }
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                m_mouseButtonStates[LEFT] = true;
            }
            if(event.button.button == SDL_BUTTON_MIDDLE)
            {
                m_mouseButtonStates[MIDDLE] = true;
            }
            if(event.button.button == SDL_BUTTON_RIGHT)
            {
                m_mouseButtonStates[RIGHT] = true;
            }
        }
        if(event.type == SDL_MOUSEBUTTONUP)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                m_mouseButtonStates[LEFT] = false;
            }
            if(event.button.button == SDL_BUTTON_MIDDLE)
            {
                m_mouseButtonStates[MIDDLE] = false;
            }
            if(event.button.button == SDL_BUTTON_RIGHT)
            {
                m_mouseButtonStates[RIGHT] = false;
            }
        }
        if(event.type == SDL_MOUSEMOTION)
        {
            m_mousePos.SetX(event.motion.x);
            m_mousePos.setY(event.motion.y);
        }
        if(event.type == SDL_WINDOWEVENT)
        {
            if(event.window.event == SDL_WINDOWEVENT_ENTER)
            {
                m_mouse_in = true;
            }
            if(event.window.event == SDL_WINDOWEVENT_LEAVE)
            {
                m_mouse_in = false;
            }
        }

        if(event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
        {
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && m_text.length() > 0)
            {
                m_text = m_text.substr(0,m_text.length()-1);
            }
            else if(event.type == SDL_TEXTINPUT)
            {
                m_text += event.text.text;
            }
            else if(event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
            {
                SDL_SetClipboardText(m_text.c_str());
            }
            else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
            {
                m_text = SDL_GetClipboardText();
            }
        }
    }
}

bool Input::MouseInScreen()
{
    return m_mouse_in;
}

bool Input::getMouseButtonState(int buttonNumber)
{
    return m_mouseButtonStates[buttonNumber];
}

bool Input::isKeyDown(SDL_Scancode key)
{
    if(m_keystates != 0)
    {
        if(m_keystates[key] == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

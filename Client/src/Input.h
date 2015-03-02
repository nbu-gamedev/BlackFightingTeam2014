#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include"Defines.h"
#include "Vector2D.h"
#include <vector>

enum mouse_buttons{LEFT = 0,MIDDLE = 1,RIGHT = 2};

class Input
{
public:
    static Input* Instance()
    {
        if(s_Instance == 0)
        {
            s_Instance = new Input();
        }
        return s_Instance;
    }
    void Update();
    void Clean();

    void InitJoysticks();

    bool getMouseButtonState(int buttonNumber);
    Vector2D getMousePosition()
    {
        return m_mousePos;
    }

    bool isKeyDown(SDL_Scancode key);
    bool MouseInScreen();

    void StartTextInput(){SDL_StartTextInput();}
    void StopTextInput(){SDL_StopTextInput();}

    std::string getTextInput(){ return m_text;}
    void setTextInput(std::string text){m_text = text;}

private:
    Input();
    ~Input();
    SDL_Event event;
    static Input* s_Instance;

    std::vector<SDL_Joystick*>m_joysticks;
    bool m_JoystickInit;

    std::vector<bool> m_mouseButtonStates;
    Vector2D m_mousePos;

    const Uint8* m_keystates;
    bool m_mouse_in;

    std::string m_text;
    bool m_btextInput;
};
typedef Input Input;



#endif // INPUT_H_INCLUDED

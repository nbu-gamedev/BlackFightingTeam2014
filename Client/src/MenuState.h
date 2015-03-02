#ifndef MENUSTATE_H_INCLUDED
#define MENUSTATE_H_INCLUDED

#include "StateManager.h"
#include "Elements.h"

class MenuState:public MenuState
{
public:
    virtual void Update(){};
    virtual void Draw(){};

    virtual bool Enter(){};
    virtual bool Exit(){};
    virtual std::string getState()const {return s_menuID;}

};


#endif // MENUSTATE_H_INCLUDED

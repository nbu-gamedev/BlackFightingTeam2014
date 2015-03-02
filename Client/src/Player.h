#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Vector2D.h"

class Player2
{
public:
    Player2();
    ~Player2();
    void Init();
    void HandleInput();
    void Update();
    void Collussion();
    void Draw();
};

#endif // PLAYER_H_INCLUDED

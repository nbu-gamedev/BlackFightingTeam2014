#include "Vector2D.h"

Vector2D::Vector2D(){
    m_x = 0;
    m_y = 0;
};

Vector2D::Vector2D(float x,float y){
    m_x = x;
    m_y = y;
}

void Vector2D::SetXY(float x,float y){
    m_x = x;
    m_y = y;
}

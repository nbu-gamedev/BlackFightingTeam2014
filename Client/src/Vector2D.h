#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED
#include <stdio.h>
#include <math.h>
#define PI 3.14159265

class Vector2D
{
public:
    float m_x;
    float m_y;

    Vector2D();
    Vector2D(float x,float y);
    void SetXY(float x,float y);
    void SetX(float x){m_x = x;}
    void setY(float y){m_y = y;}
    float getX(){return m_x;}
    float getY(){return m_y;}

    inline Vector2D& operator +=(const Vector2D& Ve2)
    {
        m_x += Ve2.m_x;
        m_y += Ve2.m_y;
        return (*this);
    }

   inline Vector2D& operator -=(const Vector2D& Ve2)
    {
        m_x -= Ve2.m_x;
        m_y -= Ve2.m_y;
        return (*this);
    }

   inline Vector2D& operator *=(const Vector2D& Ve2)
    {
        m_x *= Ve2.m_x;
        m_y *= Ve2.m_y;
        return (*this);
    }
    inline Vector2D& operator /=(const Vector2D& Ve2)
    {
        m_x /= Ve2.m_x;
        m_y /= Ve2.m_y;
        return (*this);
    }

    inline Vector2D operator+(const Vector2D& Ve2)const
    {
        return Vector2D(this->m_x+Ve2.m_x,this->m_y+Ve2.m_y);
    }
    inline Vector2D operator-(const Vector2D& Ve2)const
    {
        return Vector2D(this->m_x - Ve2.m_x,this->m_y - Ve2.m_y);
    }
    inline Vector2D operator*(const Vector2D& Ve2)const
    {
        return Vector2D(this->m_x*Ve2.m_x,this->m_y*Ve2.m_y);
    }

    inline Vector2D& operator=(const Vector2D& Ve2)
    {
        m_x = Ve2.m_x;
        m_y = Ve2.m_y;
        return *this;
    }

    inline bool operator==(const Vector2D& Ve2)
    {
        return ((m_x == Ve2.m_x)&&(m_y = Ve2.m_y));
    }
    inline bool operator!=(const Vector2D& Ve2)
    {
        return !((m_x == Ve2.m_x)&&(m_y = Ve2.m_y));
    }

    inline Vector2D& operator*=(const float scalar)
    {
        m_x *= scalar;
        m_y *= scalar;
        return *this;
    }
    inline Vector2D& operator/=(const float scalar)
    {
        m_x /= scalar;
        m_y /= scalar;
        return *this;
    }

    inline const Vector2D operator*(const float scalar)
    {
        return Vector2D(this->m_x*scalar,this->m_y*scalar);
    }
    inline const Vector2D operator/(const float scalar)
    {
        return Vector2D(this->m_x/scalar,this->m_y/scalar);
    }

    inline Vector2D& operator+(const float scalar)
    {
        m_x += scalar;
        m_y += scalar;
        return *this;
    }
    inline Vector2D& operator-(const float scalar)
    {
        m_x -= scalar;
        m_y -= scalar;
        return *this;
    }

    inline float CollisionAngle(Vector2D other)
    {
        return atan2(m_y,m_x)*180/PI;
    }
    inline void Rotate(const float angle)
    {
        m_x = cos(angle*PI/180);
        m_y = sin(angle*PI/180);
    }
    inline void ChangeAngle(const float angle)
    {
        m_x = acos(angle*PI/180);
        m_y = asin(angle*PI/180);
    }

    inline float Length()
    {
        return sqrt((m_x*m_x) + (m_y*m_y));
    }

    inline float DotProduction(Vector2D other)
    {
        return (m_x * other.m_x) + (m_y * other.m_y);
    }

};

#endif // VECTOR_H_INCLUDED

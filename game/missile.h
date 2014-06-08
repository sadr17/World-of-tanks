#ifndef MISSILE_H
#define MISSILE_H
#include <QGLWidget>
#include <math.h>
#include "tank.h"

class Missile
{
public:
    Missile();
    Missile(GLfloat x = 0, GLfloat y = 0, GLfloat direction = 0);
    GLfloat getXPos();
    GLfloat getYPos();
    void setXPos(GLfloat value);
    void setYPos(GLfloat value);
    void setAngle(GLfloat value);
    void print();
    void move();
private:
    GLfloat xPos, yPos;
    GLfloat color[4];
    GLfloat angle;
    GLfloat speed;
};

#endif // MISSILE_H

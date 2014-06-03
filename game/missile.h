#ifndef MISSILE_H
#define MISSILE_H
#include <QGLWidget>
#include <math.h>

class Missile
{
public:
    Missile();
    Missile(int id, GLfloat x = 0, GLfloat y = 0, GLfloat direction = 0);
    GLfloat getXPos();
    GLfloat getYPos();
    void setXPos(GLfloat value);
    void setYPos(GLfloat value);
    int tankID;
    void move();
    bool canMove(GLfloat top, GLfloat right, GLfloat bottom, GLfloat left);
    void print();
private:
    GLfloat xPos, yPos;
    GLfloat speed;
    GLfloat color[4];
    GLfloat angle;
};

#endif // MISSILE_H

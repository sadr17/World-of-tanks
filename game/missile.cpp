#include "missile.h"

Missile::Missile()
{
    xPos = yPos = 0;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
    angle = 0;
    speed = 0.7;
}

Missile::Missile(GLfloat x, GLfloat y, GLfloat direction)
{
    xPos = x;
    yPos = y;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
    angle = direction;
    speed = 0.7;
}
GLfloat Missile::getXPos()
{
    return xPos;
}

GLfloat Missile::getYPos()
{
    return yPos;
}

void Missile::setXPos(GLfloat value)
{
    xPos = value;
}

void Missile::setAngle(GLfloat value)
{
    angle = value;
}

void Missile::setYPos(GLfloat value)
{
    yPos = value;
}

void Missile::move()
{
    xPos += sin(angle*M_PI/180)*speed;
    yPos += cos(angle*M_PI/180)*speed;
}

void Missile::print()
{
    glPushMatrix();
        glTranslatef(xPos, yPos, 0);
        glBegin(GL_POLYGON);
        glColor4fv(color);
        glVertex2f(-0.09,0.09);
        glVertex2f(0.09,0.09);
        glVertex2f(0.09,-0.09);
        glVertex2f(-0.09,-0.09);
        glEnd();
    glPopMatrix();
}

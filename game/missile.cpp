#include "missile.h"

Missile::Missile()
{
    xPos = yPos = 0;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
}

Missile::Missile(GLfloat x, GLfloat y)
{
    xPos = x;
    yPos = y;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
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

void Missile::setYPos(GLfloat value)
{
    yPos = value;
}

void Missile::print()
{
    glPushMatrix();
        glTranslatef(xPos, yPos, 0);
        glBegin(GL_POLYGON);
        glColor4fv(color);
        glVertex2f(-0.15,0.15);
        glVertex2f(0.15,0.15);
        glVertex2f(0.15,-0.15);
        glVertex2f(-0.15,-0.15);
        glEnd();
    glPopMatrix();
}

#include "missile.h"

Missile::Missile()
{
    tankID = 0;
    xPos = yPos = angle = 0;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
    speed = 0.5;
}

Missile::Missile(int id, GLfloat x, GLfloat y, GLfloat direction)
{
    tankID = id;
    speed = 0.5;
    xPos = x;
    yPos = y;
    angle = direction;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
}

void Missile::move()
{
    xPos += sin(angle*M_PI/180)*speed;
    yPos += cos(angle*M_PI/180)*speed;
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

bool Missile::canMove(GLfloat top, GLfloat right, GLfloat bottom, GLfloat left)
{
    GLfloat xCheck = xPos + sin(angle*M_PI/180)*speed;
    GLfloat yCheck = yPos + cos(angle*M_PI/180)*speed;
    return xCheck > left && xCheck < right && yCheck > bottom && yCheck < top;
}

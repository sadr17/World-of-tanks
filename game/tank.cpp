#include "tank.h"

Tank::Tank()
{
    id = 0;
    color[0] = 1;
    color[1] = 0;
    color[2] = 0;
    color[3] = 1;
    cannonColor[0] = 0;
    cannonColor[1] = 0;
    cannonColor[2] = 0;
    cannonColor[3] = 1;
    rotation = 0;
    xPos = 0;
    yPos = 0;
    cannonRotation = 0;
}

Tank::Tank(int i)
{
    id = i;
    color[0] = 1;
    color[1] = 0;
    color[2] = 0;
    color[3] = 1;
    cannonColor[0] = 0;
    cannonColor[1] = 0;
    cannonColor[2] = 0;
    cannonColor[3] = 1;
    rotation = 0;
    cannonRotation = 0;
    xPos = 0;
    yPos = 0;
}

void Tank::print()
{
        glPushMatrix();
            glTranslatef(xPos,yPos,0);
            glRotatef(-rotation,0,0,1);
            glPushMatrix();
                glRotatef(-cannonRotation,0,0,1);
                glBegin(GL_POLYGON);
                glColor4fv(cannonColor);
                glVertex2d(-0.6,0.6);
                glVertex2d(0.6,0.6);
                glVertex2d(0.6,-0.6);
                glVertex2d(-0.6,-0.6);
                glEnd();
                glBegin(GL_POLYGON);
                glColor4fv(cannonColor);
                glVertex2d(-0.2, 0);
                glVertex2d(-0.2, 2.0);
                glVertex2d(0.2,2.0);
                glVertex2d(0.2,0);
                glEnd();
            glPopMatrix();
            glPushMatrix();
                glBegin(GL_POLYGON);
                glColor4fv(color);
                glVertex2d(-1,1);
                glVertex2d(0,1.5);
                glVertex2d(1,1);
                glVertex2d(1,-1);
                glVertex2d(0.8,-1.2);
                glVertex2d(-0.8,-1.2);
                glVertex2d(-1,-1);
                glEnd();
            glPopMatrix();
        glPopMatrix();
}

GLfloat Tank::getRotation()
{
    return rotation;
}

void Tank::setRotation(GLfloat value)
{
    rotation = value;
}

void Tank::setPos(GLfloat x, GLfloat y)
{
    xPos = x;
    yPos = y;
}

void Tank::move(GLfloat speed)
{
    xPos += sin(rotation*M_PI/180)*speed;
    yPos += cos(rotation*M_PI/180)*speed;
}

void Tank::rotate(GLfloat value)
{
    rotation = (int)(rotation + value)%360;
}

GLfloat Tank::getXPos()
{
    return xPos;
}

GLfloat Tank::getYPos()
{
    return yPos;
}

GLfloat Tank::getCannonRotation()
{
    return cannonRotation;
}

void Tank::rotateCannon(GLfloat value)
{
    cannonRotation = (int)(cannonRotation + value)%360;
}

bool Tank::canMove(GLfloat speed, GLfloat top, GLfloat right, GLfloat bottom, GLfloat left, GLfloat offset)
{
    GLfloat xCheck = xPos + sin(rotation*M_PI/180)*speed;
    GLfloat yCheck = yPos + cos(rotation*M_PI/180)*speed;
    return xCheck > (left + offset) && xCheck < (right - offset) && yCheck > (bottom + offset) && yCheck < (top - offset);
}

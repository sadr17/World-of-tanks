#include "tank.h"

Tank::Tank(int id, float x, float y, float rot, QObject * parent) :
    BaseMapObject(x, y, rot, parent)
{
    ableToShoot = true;
    this->id = id;
    color[0] = 1;
    color[1] = id % 4;
    color[2] = id % 3;
    color[3] = 1;
    cannonColor[0] = 0;
    cannonColor[1] = 0;
    cannonColor[2] = 0;
    cannonColor[3] = 1;
    cannonRotation = 0;
}

bool Tank::canShoot()
{
    return ableToShoot;
}

void Tank::canShoot(bool value)
{
    ableToShoot = value;
}

void Tank::print()
{
        glPushMatrix();
            glTranslatef(xPos,yPos,0);
            glRotatef(-angle,0,0,1);
            glPushMatrix();
                glRotatef(-cannonRotation,0,0,1);
                glBegin(GL_POLYGON);
                glColor4fv(cannonColor);
                glVertex2d(-0.36,0.36);
                glVertex2d(0.36,0.36);
                glVertex2d(0.36,-0.36);
                glVertex2d(-0.36,-0.36);
                glEnd();
                glBegin(GL_POLYGON);
                glColor4fv(cannonColor);
                glVertex2d(-0.12, 0);
                glVertex2d(-0.12, 1.2);
                glVertex2d(0.12,1.2);
                glVertex2d(0.12,0);
                glEnd();
            glPopMatrix();
            glPushMatrix();
                glBegin(GL_POLYGON);
                glColor4fv(color);
                glVertex2d(-0.6,0.6);
                glVertex2d(0,0.9);
                glVertex2d(0.6,0.6);
                glVertex2d(0.6,-0.6);
                glVertex2d(0.48,-0.72);
                glVertex2d(-0.48,-0.72);
                glVertex2d(-0.6,-0.6);
                glEnd();
            glPopMatrix();
        glPopMatrix();
}

void Tank::move(GLfloat speed)
{
    xPos += sin(angle*M_PI/180)*speed;
    yPos += cos(angle*M_PI/180)*speed;
}

void Tank::rotate(GLfloat value)
{
    angle = (int)(angle + value)%360;
}

GLfloat Tank::getCannonRotation()
{
    return cannonRotation;
}

void Tank::setCannonRotation(GLfloat value)
{
    cannonRotation = value;
}

void Tank::rotateCannon(GLfloat value)
{
    cannonRotation = (int)(cannonRotation + value)%360;
}

bool Tank::canMove(GLfloat speed, QList<Tank *> tanksList, GLfloat top, GLfloat right, GLfloat bottom, GLfloat left, GLfloat offset)
{
    GLfloat radius = 0.75;
    GLfloat xCheck = xPos + sin(angle*M_PI/180)*speed;
    GLfloat yCheck = yPos + cos(angle*M_PI/180)*speed;
    for(int i = 0; i < tanksList.size(); ++i)
    {
        int currentId = tanksList.at(i)->id;
        if(currentId != id)
        {
            GLfloat xVector = xCheck - tanksList.at(i)->getXPos();
            GLfloat yVector = yCheck - tanksList.at(i)->getYPos();
            GLfloat vector = sqrt(xVector*xVector + yVector*yVector);
            if(vector <= radius*2)
                return false;
        }
    }

//    for(int i = 0; i < map.size(); ++i)
//    {
//        GLfloat xVector = xCheck - map.at(i)->getXPos();
//        GLfloat yVector = yCheck - map.at(i)->getYPos();
//        GLfloat vector = sqrt(xVector*xVector + yVector*yVector);
//        if(vector <= radius + map.at(i)->getAngle())
//            return false;
//    }


    return xCheck > (left + offset) && xCheck < (right - offset) && yCheck > (bottom + offset) && yCheck < (top - offset);
}

void Tank::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
    color[0] = red/255;
    color[1] = green/255;
    color[2] = blue/255;
}

void Tank::setCannonColor(GLfloat red, GLfloat green, GLfloat blue)
{
    cannonColor[0] = red/255;
    cannonColor[1] = green/255;
    cannonColor[2] = blue/255;
}

#include "missile.h"

Missile::Missile()
{
    xPos = yPos = 0;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
    angle = 0;
    speed = 0.7;
}

Missile::Missile(int id, GLfloat x, GLfloat y, GLfloat direction)
{
    tankID = id;
    xPos = x;
    yPos = y;
    color[0] = color[1] = color[2] = 0;
    color[3] = 1;
    angle = direction;
    speed = 0.7;
}

int Missile::hit(QList<Tank *> tanksList)
{
    double radius = 0.6;
    double missileRadius = 0.1;
    for(int i = 0; i < tanksList.size(); ++i)
    {
        int tankId = tanksList.at(i)->id;
        if(tankId != tankID)
        {
            double xVector = xPos - tanksList.at(i)->getXPos();
            double yVector = yPos - tanksList.at(i)->getYPos();
            double vector = sqrt(xVector*xVector + yVector*yVector);
            if(vector <= radius + missileRadius)
                return tankId;
        }
    }
    return -1;
}

bool Missile::canMove(double top, double right, double bottom, double left, QList<Obstacle *> map)
{
    double xCheck = xPos + sin(angle*M_PI/180)*speed;
    double yCheck = yPos + cos(angle*M_PI/180)*speed;
    for(int i = 0; i < map.size(); ++i)
    {
        double xVector = xCheck - map.at(i)->getX();
        double yVector = yCheck - map.at(i)->getY();
        double vector = sqrt(xVector*xVector + yVector*yVector);
        if(vector <= 0.1 + map.at(i)->getRad())
            return false;
    }
    return xCheck > left && xCheck < right && yCheck > bottom && yCheck < top;
}

GLfloat Missile::getXPos()
{
    return xPos;
}

GLfloat Missile::getYPos()
{
    return yPos;
}

GLfloat Missile::getAngle()
{
    return this->angle;
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

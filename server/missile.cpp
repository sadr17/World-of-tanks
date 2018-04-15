#include "missile.h"

Missile::Missile()
{
    speed = xPos = yPos = angle = 0;
    tankID = 0;
}

Missile::Missile(int id, double x, double y, double direction)
{
    speed = 0.7;
    tankID = id;
    xPos = x;
    yPos = y;
    angle = direction;
}

void Missile::setXPos(double value)
{
    xPos = value;
}

void Missile::setYPos(double value)
{
    yPos = value;
}

double Missile::getXPos()
{
    return xPos;
}

double Missile::getYPos()
{
    return yPos;
}

double Missile::getDirection()
{
    return angle;
}

void Missile::move()
{
    xPos += sin(angle*M_PI/180)*speed;
    yPos += cos(angle*M_PI/180)*speed;
}

bool Missile::canMove(double top, double right, double bottom, double left, QList<Obstacle *> *map)
{
    double xCheck = xPos + sin(angle*M_PI/180)*speed;
    double yCheck = yPos + cos(angle*M_PI/180)*speed;
    for(int i = 0; i < map->size(); ++i)
    {
        double xVector = xCheck - map->at(i)->getX();
        double yVector = yCheck - map->at(i)->getY();
        double vector = sqrt(xVector*xVector + yVector*yVector);
        if(vector <= 0.1 + map->at(i)->getRad())
            return false;
    }
    return xCheck > left && xCheck < right && yCheck > bottom && yCheck < top;
}

int Missile::hit(QList<Tank *> *tanksList)
{
    double radius = 0.6;
    double missileRadius = 0.1;
    for(int i = 0; i < tanksList->size(); ++i)
    {
        if(i != tankID)
        {
            double xVector = xPos - tanksList->at(i)->getXPos();
            double yVector = yPos - tanksList->at(i)->getYPos();
            double vector = sqrt(xVector*xVector + yVector*yVector);
            if(vector <= radius + missileRadius)
                return i;
        }
    }
    return -1;
}

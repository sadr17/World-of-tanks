#include "tank.h"

Tank::Tank()
{
    id = 0;
    xPos = 0;
    yPos = 0;
    rotation = 0;
    cannonRotation = 0;
}

Tank::Tank(int i, double x, double y, double rot)
{
    id = i;
    xPos = x;
    yPos = y;
    rotation = rot;
    cannonRotation = 0;
}

double Tank::getRotation()
{
    return rotation;
}

double Tank::getXPos()
{
    return xPos;
}

double Tank::getYPos()
{
    return yPos;
}

void Tank::setPos(double x, double y)
{
    xPos = x;
    yPos = y;
}

void Tank::setRotation(double value)
{
    rotation = value;
}

void Tank::setCannonRotation(double value)
{
    cannonRotation = value;
}

double Tank::getCannonRotation()
{
    return cannonRotation;
}



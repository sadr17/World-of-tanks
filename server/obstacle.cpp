#include "obstacle.h"

Obstacle::Obstacle()
{
    x = y = rad = 0;
}

Obstacle::Obstacle(double xPos, double yPos, double radius)
{
    x = xPos;
    y = yPos;
    rad = radius;
}

QString Obstacle::getData()
{
    return QString::number(x) + " " + QString::number(y) + " " + QString::number(rad);
}

double Obstacle::getX()
{
    return x;
}

double Obstacle::getY()
{
    return y;
}

double Obstacle::getRad()
{
    return rad;
}

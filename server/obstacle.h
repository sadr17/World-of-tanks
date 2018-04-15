#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QString>

class Obstacle
{
public:
    Obstacle();
    Obstacle(double xPos, double yPos, double radius);
    QString getData();
    double getX();
    double getY();
    double getRad();
private:
    double x, y, rad;
};

#endif // OBSTACLE_H

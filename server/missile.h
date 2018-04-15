#ifndef MISSILE_H
#define MISSILE_H
#include "tank.h"
#include "math.h"
#include "obstacle.h"
#include <QList>

class Missile
{
public:
    Missile();
    Missile(int id, double x = 0, double y = 0, double direction = 0);
    void setXPos(double value);
    void setYPos(double value);
    double getXPos();
    double getYPos();
    double getDirection();
    int tankID;
    void move();
    int hit(QList<Tank*> *tanksList);
    bool canMove(double top, double right, double bottom, double left, QList<Obstacle *> *map);
private:
    double speed;
    double xPos, yPos;
    double angle;
};

#endif // MISSILE_H

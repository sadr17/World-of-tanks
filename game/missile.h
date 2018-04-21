#ifndef MISSILE_H
#define MISSILE_H
#include "basemapobject.h"
#include "tank.h"

class Missile : public BaseMapObject
{
    Q_OBJECT
public:
    Missile(Tank * tank);

    int getTankId();
    void move();
    int hit(QList<Tank *> tanksList);
    bool canMove(double top, double right, double bottom, double left, QList<Obstacle *> map);

    void print();
private:
    float color[4];
    float speed;
    int tankID;
};

#endif // MISSILE_H

#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <basemapobject.h>

class Obstacle : public BaseMapObject
{
    Q_OBJECT
public:
    Obstacle(float x, float y, float angle, QObject * parent = nullptr);
    void print();
};

#endif // OBSTACLE_H

#ifndef BASEMAPOBJECT_H
#define BASEMAPOBJECT_H

#include <QObject>
#include <QUuid>

class BaseMapObject : public QObject
{
    Q_OBJECT
public:
    BaseMapObject(float x, float y, float angle, QObject * parent = nullptr);
    QUuid getUid();

    float getXPos();
    float getYPos();
    float getAngle();

    void setPos(float x, float y);
    void setXPos(float value);
    void setYPos(float value);
    void setAngle(float value);

    virtual void print() = 0;

protected:
    float xPos, yPos;
    float angle;
    QUuid uid;
};

#endif // BASEMAPOBJECT_H

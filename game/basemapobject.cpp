#include "basemapobject.h"

BaseMapObject::BaseMapObject(float x, float y, float angle, QObject * parent) : QObject(parent)
{
    this->xPos = x;
    this->yPos = y;
    this->angle = angle;
}

QUuid BaseMapObject::getUid()
{
    if(this->uid.isNull())
        this->uid = QUuid::createUuid();
  return uid;
}

float BaseMapObject::getXPos()
{
    return this->xPos;
}

float BaseMapObject::getYPos()
{
    return this->yPos;
}

float BaseMapObject::getAngle()
{
    return this->angle;
}

void BaseMapObject::setPos(float x, float y)
{
    this->setXPos(x);
    this->setYPos(y);
}

void BaseMapObject::setXPos(float value)
{
    this->xPos = value;
}

void BaseMapObject::setYPos(float value)
{
    this->yPos = value;
}

void BaseMapObject::setAngle(float value)
{
    this->angle = value;
}

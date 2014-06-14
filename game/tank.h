#ifndef TANK_H
#define TANK_H
#include <QGLWidget>
#include <math.h>

class Tank
{
public:
    Tank();
    Tank(int i);
    void print();
    void setRotation(GLfloat value);
    void setCannonRotation(GLfloat value);
    void rotate(GLfloat value);
    void rotateCannon(GLfloat value);
    void setPos(GLfloat x, GLfloat y);
    GLfloat getRotation();
    GLfloat getCannonRotation();
    void move(GLfloat speed);
    GLfloat getXPos();
    GLfloat getYPos();
    bool hasAmmo();
    QString ammoText();
    bool canShoot();
    void canShoot(bool value);
    void takeAmmo(int value);
    bool canMove(GLfloat speed,QList<Tank *> *tanksList, GLfloat top, GLfloat right, GLfloat bottom, GLfloat left, GLfloat offset = 0);
    int id;
private:
    GLfloat xPos, yPos;
    int ammo;
    bool ableToShoot;
    GLfloat color[4], cannonColor[4];
    GLfloat rotation, cannonRotation;
};

#endif // TANK_H

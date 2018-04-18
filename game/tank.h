#ifndef TANK_H
#define TANK_H
#include <QGLWidget>
#include <math.h>
#include <obstacle.h>

class Tank : public QObject
{
    Q_OBJECT
public:
    explicit Tank(int id, QObject * parent = nullptr);
    Tank(int id, GLfloat x, GLfloat y, GLfloat rot, QObject * parent = nullptr);
    void print();
    void setRotation(GLfloat value);
    void setCannonRotation(GLfloat value);
    void rotate(GLfloat value);
    void rotateCannon(GLfloat value);
    void setPos(GLfloat x, GLfloat y);
    void setColor(GLfloat red, GLfloat green, GLfloat blue);
    void setCannonColor(GLfloat red, GLfloat green, GLfloat blue);
    GLfloat getRotation();
    GLfloat getCannonRotation();
    void move(GLfloat speed);
    GLfloat getXPos();
    GLfloat getYPos();
    bool canShoot();
    void canShoot(bool value);
    bool canMove(GLfloat speed, QList<Tank *> tanksList, QList<Obstacle *> *map, GLfloat top, GLfloat right, GLfloat bottom, GLfloat left, GLfloat offset = 0);
    int id;
private:
    GLfloat xPos, yPos;
    bool ableToShoot;
    GLfloat color[4], cannonColor[4];
    GLfloat rotation, cannonRotation;
};

#endif // TANK_H

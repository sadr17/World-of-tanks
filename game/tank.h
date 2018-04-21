#ifndef TANK_H
#define TANK_H
#include <QGLWidget>
#include <math.h>
#include <basemapobject.h>
#include <obstacle.h>

class Tank : public BaseMapObject
{
    Q_OBJECT
public:
    Tank(int id, float x, float y, float angle, QObject * parent = nullptr);

    void setCannonRotation(GLfloat value);
    void rotate(GLfloat value);
    void rotateCannon(GLfloat value);
    void setColor(GLfloat red, GLfloat green, GLfloat blue);
    void setCannonColor(GLfloat red, GLfloat green, GLfloat blue);
    GLfloat getCannonRotation();
    void move(GLfloat speed);
    bool canShoot();
    void canShoot(bool value);
    bool canMove(GLfloat speed, QList<Tank *> tanksList, GLfloat top, GLfloat right, GLfloat bottom, GLfloat left, GLfloat offset = 0);

    int getId() { return this->id; }
    void print();

private:
    int id;
    bool ableToShoot;
    GLfloat color[4], cannonColor[4];
    GLfloat cannonRotation;
};

#endif // TANK_H

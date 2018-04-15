#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGLWidget>
#include "math.h"

class Obstacle
{
public:
    Obstacle();
    Obstacle(GLfloat xPos, GLfloat yPos, GLfloat radius);
    void print();
    GLfloat getX();
    GLfloat getY();
    GLfloat getRad();
private:
    GLfloat x, y;
    GLfloat rad;
};

#endif // OBSTACLE_H

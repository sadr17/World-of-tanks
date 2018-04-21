#include "obstacle.h"
#include <QGLWidget>
#include <math.h>

Obstacle::Obstacle(float x, float y, float angle, QObject *parent) :
    BaseMapObject(x, y, angle, parent)
{

}

void Obstacle::print()
{
    qsrand(xPos + yPos);
    int color = qrand() % 9;
    glPushMatrix();
    glBegin(GL_TRIANGLE_FAN);
    glColor3d(color * 0.1, color * 0.1, 0.1);
    glVertex2f(xPos, yPos);
    const GLfloat radians = M_PI/180;
    for(GLfloat i = 0; i < 361; i += 10)
    {
        int random = qrand()% (int)(i + 1);
        GLfloat angle = i * radians;
        glVertex2f(xPos + cos(angle + random)*this->angle, yPos + sin(angle - random)*this->angle);
    }
    glEnd();
    glPopMatrix();
}

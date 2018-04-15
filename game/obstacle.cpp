#include "obstacle.h"

Obstacle::Obstacle()
{
    x = y = rad = 0;
}

Obstacle::Obstacle(GLfloat xPos, GLfloat yPos, GLfloat radius)
{
    x = xPos;
    y = yPos;
    rad = radius;
}

void Obstacle::print()
{
    glPushMatrix();
    glBegin(GL_TRIANGLE_FAN);
    glColor3d(0.0, 0.0, 0.0);
    glVertex2f(x,y);
    const GLfloat radians = M_PI/180;
    for(GLfloat i = 0; i < 361; i += 10)
    {
        GLfloat angle = i * radians;
        glVertex2f(x + cos(angle)*rad, y + sin(angle)*rad);
    }
    glEnd();
    glPopMatrix();
}

GLfloat Obstacle::getRad()
{
    return rad;
}

GLfloat Obstacle::getX()
{
    return x;
}

GLfloat Obstacle::getY()
{
    return y;
}

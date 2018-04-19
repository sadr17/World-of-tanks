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
    qsrand(x + y);
    int color = qrand() % 9;
    glPushMatrix();
    glBegin(GL_TRIANGLE_FAN);
    glColor3d(color * 0.1, color * 0.1, 0.1);
    glVertex2f(x,y);
    const GLfloat radians = M_PI/180;
    for(GLfloat i = 0; i < 361; i += 10)
    {
        int random = qrand()% (int)(i + 1);
        GLfloat angle = i * radians;
        glVertex2f(x + cos(angle + random)*rad, y + sin(angle - random)*rad);
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

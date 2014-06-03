#include "display.h"

Display::Display(QWidget *parent) :
    QGLWidget(parent)
{
    bColor[0] = 0.7;
    bColor[1] = 0.7;
    bColor[2] = 0.7;
    bColor[3] = 1;
}

void Display::resizeGL(int w, int h)
{
    int s = qMin(w, h);
    glViewport((w-s)/2,(h-s)/2,s,s);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20,20,-20,20,4,15);
    glMatrixMode(GL_MODELVIEW);
}

void Display::initializeGL()
{
    glClearColor(bColor[0], bColor[1], bColor[2], bColor[3]);
    glEnable(GL_DEPTH_TEST);
}

void Display::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0,0,-10);
    for(int i = 0; i < playerList.size(); ++i)
    {
        playerList[i]->print();
    }
    for(int i = 0; i < missileList.size(); ++i)
    {
        missileList[i]->print();
    }

}

Display::~Display()
{
    while(!playerList.isEmpty())
        delete playerList.takeFirst();
    while(!missileList.isEmpty())
        delete missileList.takeFirst();
}

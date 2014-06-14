#include "display.h"

Display::Display(QWidget *parent) :
    QGLWidget(parent)
{
    bColor[0] = 0.7;
    bColor[1] = 0.7;
    bColor[2] = 0.7;
    bColor[3] = 1;

    mapWidth = 64;
    mapHeight = 36;

    ammoHud = "50";
}

void Display::resizeGL(int w, int h)
{
//    int s = qMin(w, h);
//    glViewport((w-s)/2,(h-s)/2,s,s);
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-(mapWidth/2),mapWidth/2,-(mapHeight/2),mapHeight/2,4,15);
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
    drawHudBar();
}

void Display::drawHudBar()
{
    glPushMatrix();
        qglColor(Qt::white);
        renderText(-31, -17.7, 0, ammoHud + "/50", QFont("Arial", 12, QFont::Normal, false));
        glBegin(GL_POLYGON);
        glColor3d(0.3,0.3,0.3);
        glVertex2d(-32.0,-16.8);
        glVertex2d(32.0,-16.8);
        glVertex2d(32.0,-18.0);
        glVertex2d(-32.0,-18.0);
        glEnd();
    glPopMatrix();
}

Display::~Display()
{
    while(!playerList.isEmpty())
        delete playerList.takeFirst();
    while(!missileList.isEmpty())
        delete missileList.takeFirst();
}

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
    ammoProgress = 1.0;
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(bColor[0], bColor[1], bColor[2], bColor[3]);
    glEnable(GL_DEPTH_TEST);
}

void Display::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0,0,-10);
    for(int i = playerList.size() - 1; i >= 0; --i)
    {
        playerList[i]->print();
    }
    for(int i = missileList.size() - 1; i >= 0; --i)
    {
        missileList[i]->print();
        qDebug() << "Leci pocisk " + QString::number(i);
    }
    drawHudBar();
}

void Display::drawHudBar()
{
    // Ammo status
    qglColor(Qt::white);
    renderText(-31, -17.7, -4, ammoHud + "/50", QFont("Arial", 12, QFont::Normal, false));
    // Ammo bar
    GLdouble progress = 18 * ammoProgress;
    glBegin(GL_POLYGON);
    glColor3d(1.0 - ammoProgress, 0 + ammoProgress, 0);
    glVertex2d(-28, -17.8);
    glVertex2d(-28, -17.0);
    glVertex2d(-28 + progress, -17.0);
    glVertex2d(-28 + progress, -17.8);
    glEnd();
    // Background
    glBegin(GL_POLYGON);
    glColor4d(0.3,0.3,0.3,1.0);
    glVertex2d(-32.0,-16.8);
    glVertex2d(32.0,-16.8);
    glVertex2d(32.0,-18.0);
    glVertex2d(-32.0,-18.0);
    glEnd();
}

Display::~Display()
{
    while(!playerList.isEmpty())
        delete playerList.takeFirst();
    while(!missileList.isEmpty())
        delete missileList.takeFirst();
}

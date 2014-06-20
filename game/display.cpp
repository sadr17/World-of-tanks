#include "display.h"
#include <QPainter>

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
    drawScore = false;
    gameTimer = 0;
}

void Display::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-(mapWidth/2),mapWidth/2,-(mapHeight/2),mapHeight/2,4,15);
    glMatrixMode(GL_MODELVIEW);
}

void Display::initializeGL()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(bColor[0], bColor[1], bColor[2], bColor[3]);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Display::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0,0,-10);
    for(int i = map.size() - 1; i >= 0; --i)
    {
        map[i]->print();
    }
    for(int i = playerList.size() - 1; i >= 0; --i)
    {
        playerList[i]->print();
    }
    for(int i = missileList.size() - 1; i >= 0; --i)
    {
        missileList[i]->print();
    }
    drawHudBar();
    if (drawScore) drawScoreboard();
    drawTimer();
}

void Display::drawHudBar()
{
    QFont font("DejaVu Sans", 12, QFont::Light, false);
    glPushMatrix();
        // Ammo status
        glPushMatrix();
            qglColor(Qt::white);
            renderText(-31, -17.7, -4, ammoHud + "/50", font);
        glPopMatrix();
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
    glPopMatrix();
}

void Display::drawScoreboard()
{
    QFont font("Droid Sans", 12, QFont::Light);
    glPushMatrix();
        qglColor(Qt::white);
        renderText(-29, 11.0, -8, "Tablica wyników", font);
        glPushMatrix();
            for(int i = 0; i < scoreboard.size(); ++i)
            {
                glTranslated(0.0, -2.0, 0.0);
                renderText(-29, 11.0, -8, "ID: " + scoreboard[i]->toString(), font);
            }
        glPopMatrix();
        glTranslated(0.0, 0.0, 2.0);
        glBegin(GL_POLYGON);
        glColor4f(0, 0, 0, 0.8);
        glVertex2d(-32.0, 12.0);
        glVertex2d(-20.0, 12.0);
        glVertex2d(-20.0, -12.0);
        glVertex2d(-32.0, -12.0);
        glEnd();
    glPopMatrix();
}

void Display::drawTimer()
{
    int minutes = gameTimer/(60*1000);
    int seconds = (gameTimer/1000)%60;
    QString mins = QString::number(minutes);
    QString sec = QString::number(seconds);
    qDebug() << mins + ":" + sec;
    QFont font("DejaVu Sans", 12, QFont::Light, false);
    glPushMatrix();
        qglColor(Qt::white);
        renderText(30,-17.7, -4, mins + ":" + sec, font);
    glPopMatrix();
}

Display::~Display()
{
    while(!playerList.isEmpty())
        delete playerList.takeFirst();
    while(!missileList.isEmpty())
        delete missileList.takeFirst();
    while(!scoreboard.isEmpty())
        delete scoreboard.takeFirst();
    while(!map.isEmpty())
        delete map.takeFirst();
}

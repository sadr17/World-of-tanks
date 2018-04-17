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

    ammoProgress = 1.0;
    gameTimer = 0;
    gameStatus = false;
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
    if (!gameStatus) drawResults();
    for(int i = map.size() - 1; i >= 0; --i)
    {
        map[i]->print();
    }
    for(int i = playerList.keys().size() - 1; i >= 0; --i)
    {
        int key = playerList.keys()[i];
        playerList[key]->print();
    }
    for(int i = missileList.size() - 1; i >= 0; --i)
    {
        missileList[i]->print();
    }
    drawHudBar();
    drawSmallScoreboard();
    drawTimer();
}

void Display::drawHudBar()
{
    QFont font("Arial", 12, QFont::Light, false);
    glPushMatrix();
        qglColor(Qt::white);
        renderText(-13.0, -17.7, -8, QString::number(playerID + 1), font);
        // Ammo bar
        GLdouble progress = 18 * ammoProgress;
        glBegin(GL_POLYGON);
        glColor3d(1.0 - ammoProgress, 0 + ammoProgress, 0);
        glVertex2d(-31.5, -17.8);
        glVertex2d(-31.5, -17.0);
        glVertex2d(-31.5 + progress, -17.0);
        glVertex2d(-31.5 + progress, -17.8);
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

void Display::drawTimer()
{
    int minutes = gameTimer/(60*1000);
    int seconds = (gameTimer/1000)%60;
    QString mins = QString::number(minutes);
    QString sec = QString::number(seconds);
    QFont font("Arial", 12, QFont::Light, false);
    qglColor(Qt::white);
    renderText(30,-17.7, -4, mins + ":" + sec, font);
}

void Display::drawSmallScoreboard()
{
    QFont font("Arial", 12, QFont::Light);
    glPushMatrix();
    for(int i = 0; i < scoreboard.size(); ++i)
    {
        int id = scoreboard.keys()[i];
        id == playerID ? qglColor(Qt::green) : qglColor(Qt::white);
        renderText(-10.0, -17.7, -4, scoreboard[id]->toString(), font);
        glTranslatef(3.0, 0.0, 0.0);
    }
    glPopMatrix();
}

void Display::drawResults()
{
    QFont font("Arial", 20, QFont::Light, false);
    int winner = 0;
    for(int i = 1; i < playerList.size(); ++i)
    {
        int id = scoreboard.keys()[i];
        if(scoreboard[id]->getKills() > scoreboard[winner]->getKills())
        {
            winner = id;
        }
    }
    qglColor(Qt::white);
    renderText(-4.0, 15.0, -4, "Scoreboard: " + QString::number(winner + 1), font);
    glBegin(GL_POLYGON);
    glColor4f(0.0, 0.0, 0.0, 0.7);
    glVertex2f(-30.0, 17.0);
    glVertex2f(30.0, 17.0);
    glVertex2f(30.0, 14.0);
    glVertex2f(-30.0, 14.0);
    glEnd();
}

Display::~Display()
{
    qDeleteAll(this->playerList);
    qDeleteAll(this->missileList);
    qDeleteAll(this->scoreboard);
    qDeleteAll(this->map);
}

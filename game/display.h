#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGLWidget>
#include <QDebug>
#include "tank.h"
#include "missile.h"
#include "score.h"
#include "obstacle.h"

class Display : public QGLWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);
    QList<Tank *> playerList;
    QList<Missile *> missileList;
    QList<Score *> scoreboard;
    QList<Obstacle *> map;
    ~Display();
    GLint mapWidth;
    GLint mapHeight;
    QString ammoHud;
    GLdouble ammoProgress;
    bool drawScore;
private:
    void paintGL();
    void createMap();
    void initializeGL();
    void resizeGL(int w, int h);
    void drawHudBar();
    void drawScoreboard();
    GLfloat bColor[4];
signals:

public slots:

};

#endif // DISPLAY_H

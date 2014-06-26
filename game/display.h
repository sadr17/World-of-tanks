#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGLWidget>
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
    GLdouble ammoProgress;
    int gameTimer;
    bool gameStatus; // true - playing, false - round finished
    int playerID;
private:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    void drawHudBar();
    void drawSmallScoreboard();
    void drawTimer();
    void drawResults();
    GLfloat bColor[4];
signals:

public slots:

};

#endif // DISPLAY_H

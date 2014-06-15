#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGLWidget>
#include <QDebug>
#include "tank.h"
#include "missile.h"
#include "score.h"

class Display : public QGLWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);
    QList<Tank *> playerList;
    QList<Missile *> missileList;
    QList<Score *> scoreboard;
    ~Display();
    GLint mapWidth;
    GLint mapHeight;
    QString ammoHud;
    GLdouble ammoProgress;
    bool drawScore;
private:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    void drawHudBar();
    void drawScoreboard();
    GLfloat bColor[4];
signals:

public slots:

};

#endif // DISPLAY_H

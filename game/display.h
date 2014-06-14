#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGLWidget>
#include <QDebug>
#include "tank.h"
#include "missile.h"

class Display : public QGLWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);
    QList<Tank *> playerList;
    QList<Missile *> missileList;
    ~Display();
    GLint mapWidth;
    GLint mapHeight;
    QString ammoHud;

private:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    void drawHudBar();
    GLfloat bColor[4];
signals:

public slots:

};

#endif // DISPLAY_H

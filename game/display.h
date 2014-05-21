#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGLWidget>
#include "tank.h"

class Display : public QGLWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);
    QList<Tank *> playerList;
    ~Display();
private:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    GLfloat bColor[4];
signals:

public slots:

};

#endif // DISPLAY_H

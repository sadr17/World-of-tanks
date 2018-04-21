#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGLWidget>
#include <basemapobject.h>
#include <score.h>

class Display : public QGLWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);
    ~Display();

    void drawObjects();
    void addMapObject(BaseMapObject * obj);
    void removeMapObject(BaseMapObject * obj);
    void setAmmoProgress(float value);

    int getMapWidth();
    int getMapHeight();

    void setScoreboard(QMap<int, Score*> map);
    void setPlayerId(int id);

private:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    void drawHudBar();
    void drawSmallScoreboard();
    void drawTimer();
    void drawResults();
    GLfloat bColor[4];
    QHash<QUuid, BaseMapObject*> objects;
    QMap<int, Score *> scoreboard;

    GLint mapWidth;
    GLint mapHeight;
    GLdouble ammoProgress;
    int gameTimer;
    bool gameStatus; // true - playing, false - round finished
    int playerID;

signals:

public slots:

};

#endif // DISPLAY_H

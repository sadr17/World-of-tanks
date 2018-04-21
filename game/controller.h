#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include "tank.h"
#include "missile.h"
#include "score.h"
#include "obstacle.h"
#include <gameconnector.h>
#include <QTimer>

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QObject * parent = nullptr);
    void connectToRegistry(const QString &registry);
    void handleKey(Qt::Key key);
    void setMapSize(float width, float height);
    QMap<int, Score *> getScoreboard();
    int getPlayerId();

private slots:
    void onTimer();
    void initMap();
    void initPlayer();
    QMap<QString, double> getRandomPos();
    void fire();

    void updateReplica(int id, float x, float y, float an, float cAn);
    void fireReplica(int id);
    void killedReplica(int id, int hitId);


private:
    void movePlayer();
    void initTimer();
    void handleMissiles();
    void handleAmmoProgress();


private:
    QMap<int, Tank *> players;
    QList<Missile *> missiles;
    QMap<int, Score *> scoreboard;
    QList<Obstacle *> map;

    float mapWidth, mapHeight;

    QTimer timer;
    GameConnector * game;
    int roundTimer;

    int mt;
    bool mtON;

    int playerID;
    int timerInterval;
    double ammoProgress;

signals:
    void mapObjectAdded(BaseMapObject * object);
    void mapObjectRemoved(BaseMapObject * object);
    void refresh();
    void ammoProgressChanged(double value);
};

#endif // CONTROLLER_H

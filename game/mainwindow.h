#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QInputDialog>
#include <QTimer>
#include <QDir>
#include <QDesktopWidget>
#include <QSizePolicy>

#include "obstacle.h"
#include "gameconnector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    virtual int heightForWidth(int w) const {return w * 9/16;};

    void handleAmmoProgress();

public slots:
    void updateReplica(int id, float x, float y, float an, float cAn);
    void fireReplica(int id, float x, float y, float an);
    void killedReplica(int id, int hitId);
signals:

private slots:
    void onTimer();
    void setupMap();
    void initPlayer();
    QMap<QString, double> getRandomPos();
    void fire();

private:
    void movePlayer();
    void connectBox();
    void initTimer();
    void handleMissiles();

    Ui::MainWindow *ui;
    QTimer timer;
    GameConnector * game;
    int roundTimer;

    int mt;
    bool mtON;

    int playerID;
    int timerInterval;
    QHash<int, bool> keys;

    QList<Obstacle *> map;

};

#endif // MAINWINDOW_H

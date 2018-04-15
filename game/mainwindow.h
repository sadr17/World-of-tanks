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

signals:

private slots:
    void onTimer();

    void setupMap();
    void setupPlayers();
    void setDefaultPos();
    void fire();
private:
    Ui::MainWindow *ui;
    QTimer timer;
    int roundTimer;
    bool roundTimerEnabled;
    void connectBox();
    int mt;
    bool mtON;
    void movePlayer();
    void updateGame(QString &data);
    int playerID;
    int timerInterval;
    double defaultPosTab[4][3];
    bool keyUp, keyDown, keyRight, keyLeft, keyQ, keyE, keySpace;

    //From server code
    QList<Obstacle *> map;
};

#endif // MAINWINDOW_H

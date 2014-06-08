#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTimer>
#include "tank.h"
#include "missile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<Tank *> playersList;
    QList<Missile *> missileList;
    double defaultPosTab[4][3];
    void setDefaultPos();

    QTcpServer *server;
    QNetworkSession *networkSession;
    QList<QTcpSocket *> clients;

    void onConnectMessage(QTcpSocket *_socket);
    QString getPlayerInfo(int id);
    QString getMissileInfo(int id);
    QTimer gameTimer;
    int timerInterval;
private slots:
    void openSession();
    void onTimer();
    void newConnection();
    void clientDisconnect();
    void newInfo();
};

#endif // MAINWINDOW_H

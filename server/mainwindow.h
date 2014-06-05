#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "tank.h"

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

    QTcpServer *server;
    QNetworkSession *networkSession;
    QList<QTcpSocket *> clients;

private slots:
    void openSession();
    void newConnection();
    void clientDisconnect();
    void newInfo();
};

#endif // MAINWINDOW_H

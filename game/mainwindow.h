#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QInputDialog>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QDesktopWidget>
#include <QSizePolicy>

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
    void infoReceived();
    void onTimer();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    void connectBox();
    int mt;
    bool mtON;
    void movePlayer();
    void updateGame(QString &data);
    int playerID;
    int timerInterval;
    QTcpSocket *socket;
    bool keyUp, keyDown, keyRight, keyLeft, keyQ, keyE, keySpace;
};

#endif // MAINWINDOW_H

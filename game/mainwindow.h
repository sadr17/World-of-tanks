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

signals:

private slots:
    void on_actionConnect_triggered();
    void infoReceived();
    void onTimer();
    void on_actionWyjd_triggered();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    int mt;
    bool mtON;
    bool moved;
    void movePlayer();
    int playerID;
    QTcpSocket *socket;
    bool keyUp, keyDown, keyRight, keyLeft, keyQ, keyE;
};

#endif // MAINWINDOW_H

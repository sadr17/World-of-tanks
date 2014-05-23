#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QInputDialog>
#include <QTimer>
#include <QDir>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    bool eventFilter(QObject *object, QEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private slots:
    void on_actionConnect_triggered();
    void infoReceived();
    void onTimer();
private:
    Ui::MainWindow *ui;
    QTimer timer;
    void movePlayer();
    QSet<int> pressedKeys;
    int playerID;
    QTcpSocket *socket;
    bool keyUp, keyDown, keyRight, keyLeft, keyQ, keyE;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QInputDialog>
#include <QTimer>
#include <QDir>
#include <QDesktopWidget>
#include <QSizePolicy>

#include <controller.h>

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
    virtual int heightForWidth(int w) const { return w * 9/16; }

signals:

private:
    void connectBox();
    void movePlayer();

    Ui::MainWindow *ui;
    QHash<int, bool> keys;
    Controller * controller;

};

#endif // MAINWINDOW_H

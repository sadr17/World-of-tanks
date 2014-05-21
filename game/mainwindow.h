#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QInputDialog>
#include <QTimer>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_actionConnect_triggered();
    void infoReceived();
private:
    Ui::MainWindow *ui;
    int playerID;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H

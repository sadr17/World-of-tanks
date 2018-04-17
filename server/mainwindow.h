#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRemoteObjectRegistryHost>
#include "../build-server/rep_counter_source.h"

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
    QRemoteObjectRegistryHost *registry;
    CounterSimpleSource * counter;

private slots:
    void openSession();
};

#endif // MAINWINDOW_H

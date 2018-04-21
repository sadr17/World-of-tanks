#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../common/utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSizePolicy qsp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    this->setSizePolicy(qsp);
    setCentralWidget(ui->widget);
    this->setFixedSize(1152,720);

    connectBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    this->keys[event->key()] = true;
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    this->keys[event->key()] = false;
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::movePlayer()
{
    foreach(auto key, this->keys.keys()) {
     if (!this->keys[key])
         continue;

     this->controller->handleKey((Qt::Key)key);
    }
}

void MainWindow::connectBox()
{
    bool ok;
    QString connectionData = QInputDialog::getText(this,
                                                   tr("Enter node address"),
                                                   tr("Enter address of node"),
                                                   QLineEdit::Normal,
                                                   Utils::getCurrentIpAddress(), &ok);

    if (ok && !connectionData.isEmpty())
    {
        this->controller = new Controller(this);
        this->controller->setMapSize(ui->widget->getMapWidth(), ui->widget->getMapHeight());
        this->controller->connectToRegistry(connectionData);
        connect(controller, &Controller::refresh, [this](){
            this->movePlayer();
            ui->widget->setPlayerId(controller->getPlayerId());
            ui->widget->setScoreboard(controller->getScoreboard());
            ui->widget->updateGL();
        });

        connect(this->controller, &Controller::mapObjectAdded, [this](BaseMapObject * obj){
            ui->widget->addMapObject(obj);
        });

        connect(this->controller, &Controller::mapObjectRemoved, [this](BaseMapObject * obj){
            ui->widget->removeMapObject(obj);
        });

        connect(this->controller, &Controller::ammoProgressChanged, [this](float value){
            ui->widget->setAmmoProgress(value);
        });
    }
    else
    {
        exit(0);
    }
}



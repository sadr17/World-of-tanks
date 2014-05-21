#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setWindowState(Qt::WindowFullScreen);
    setCentralWidget(ui->widget);
    playerID = 0;
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(infoReceived()));
    on_actionConnect_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::infoReceived()
{
    QTextStream in(socket);
    int listSize = ui->widget->playerList.size();
    if(ui->widget->playerList.isEmpty()) listSize = 1;
    for(int i = 0; i < listSize; ++i)
    {
        QString message = in.readLine();
        QStringList info = message.split(" ", QString::SkipEmptyParts);
        if(info[0] == "NewPlayer:")
        {
            int idInfo = info[1].toInt();
            ui->widget->playerList.append(new Tank(idInfo));
            GLfloat xPosInfo = (GLfloat)info[2].toDouble();
            GLfloat yPosInfo = (GLfloat)info[3].toDouble();
            GLfloat rotInfo = (GLfloat)info[4].toDouble();
            ui->widget->playerList[idInfo]->setPos(xPosInfo , yPosInfo);
            ui->widget->playerList[idInfo]->setRotation(rotInfo);
            break;
        }
        else if(info[0] == "PlayersOnline:")
        {
            playerID = info[1].toInt();
            for(int i = 0; i <= playerID; ++i)
            {
                ui->widget->playerList.append(new Tank(i));
            }
        }
        else
        {
            int idInfo = info[0].toInt();
            GLfloat xPosInfo = (GLfloat)info[1].toDouble();
            GLfloat yPosInfo = (GLfloat)info[2].toDouble();
            GLfloat rotInfo = (GLfloat)info[3].toDouble();
            ui->widget->playerList[idInfo]->setPos(xPosInfo , yPosInfo);
            ui->widget->playerList[idInfo]->setRotation(rotInfo);
        }
    }
    ui->widget->updateGL();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QTextStream out(socket);
    switch(event->key())
    {
    case Qt::Key_Up:
        ui->widget->playerList[playerID]->move(0.3);
        ui->widget->updateGL();
        break;
    case Qt::Key_Down:
        ui->widget->playerList[playerID]->move(-0.2);
        ui->widget->updateGL();
        break;
    case Qt::Key_Right:
        ui->widget->playerList[playerID]->rotate(10);
        ui->widget->updateGL();
        break;
    case Qt::Key_Left:
        ui->widget->playerList[playerID]->rotate(-10);
        ui->widget->updateGL();
        break;
    case Qt::Key_Q:
        ui->widget->playerList[playerID]->rotateCannon(10);
        ui->widget->updateGL();
        break;
    case Qt::Key_E:
        ui->widget->playerList[playerID]->rotateCannon(-10);
        ui->widget->updateGL();
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }

    out << QString::number(ui->widget->playerList[playerID]->id)
           + " " + QString::number(ui->widget->playerList[playerID]->getXPos())
           + " " + QString::number(ui->widget->playerList[playerID]->getYPos())
           + " " + QString::number(ui->widget->playerList[playerID]->getRotation()) << endl;
}

void MainWindow::on_actionConnect_triggered()
{
    bool ok;
    QString connectionData = QInputDialog::getText(this, tr("Podaj adres serwera"), tr("Podaj adres IP oraz port:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok && !connectionData.isEmpty())
    {
        QStringList data = connectionData.split(":", QString::SkipEmptyParts);
        socket->connectToHost(data[0], data[1].toInt());
    }
}

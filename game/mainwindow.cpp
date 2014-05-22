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
    timer.setSingleShot(false);
    timer.setInterval(30);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    while(!ui->widget->playerList.isEmpty())
        delete ui->widget->playerList.takeFirst();
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
    switch(event->key())
    {
    case Qt::Key_Up:
        if(!ui->widget->playerList[playerID]->canMove(0.3, 20, 20, -20, -20, 1.5)) return;
        keyUp = true;
        qDebug() << "Key UP pressed";
        break;
    case Qt::Key_Down:
        if(!ui->widget->playerList[playerID]->canMove(-0.2, 20, 20, -20, -20, 1.5)) return;
        keyDown = true;
        qDebug() << "Key DOWN pressed";
        break;
    case Qt::Key_Right:
        keyRight = true;
        qDebug() << "Key RIGHT pressed";
        break;
    case Qt::Key_Left:
        keyLeft = true;
        qDebug() << "Key LEFT pressed";
        break;
    case Qt::Key_Q:
        keyQ = true;
        qDebug() << "Key Q pressed";
        break;
    case Qt::Key_E:
        keyE = true;
        qDebug() << "Key E pressed";
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
    {
        switch(event->key())
        {
            case Qt::Key_Up:
                keyUp = false;
                qDebug() << "Key UP released";
                break;
            case Qt::Key_Down:
                keyDown = false;
                qDebug() << "Key DOWN released";
                break;
            case Qt::Key_Left:
                keyLeft = false;
                qDebug() << "Key LEFT released";
                break;
            case Qt::Key_Right:
                keyRight = false;
                qDebug() << "Key RIGHT released";
                break;
            case Qt::Key_Q:
                keyQ = false;
                qDebug() << "Key Q released";
                break;
            case Qt::Key_E:
                keyE = false;
                qDebug() << "Key E released";
                break;
        }
    }
}

void MainWindow::movePlayer()
{
    QTextStream out(socket);

    if(keyUp)
        ui->widget->playerList[playerID]->move(0.2);
    else if(keyDown)
        ui->widget->playerList[playerID]->move(-0.1);
    if(keyLeft)
        ui->widget->playerList[playerID]->rotate(-5);
    else if(keyRight)
        ui->widget->playerList[playerID]->rotate(5);
    if(keyE)
        ui->widget->playerList[playerID]->rotateCannon(-3);
    else if(keyQ)
        ui->widget->playerList[playerID]->rotateCannon(3);

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

void MainWindow::onTimer()
{
    movePlayer();
    ui->widget->updateGL();
}

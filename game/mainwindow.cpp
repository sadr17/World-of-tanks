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
    moved = false;
    keyUp = keyDown = keyLeft = keyRight = keyE = keyQ = false;
    timer.setSingleShot(false);
    timer.setInterval(30);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer.start();
    missileTimer.setSingleShot(true);
    missileTimer.setInterval(2000);
    connect(&missileTimer,SIGNAL(timeout()),this,SLOT(onMissileTimer()));
}

MainWindow::~MainWindow()
{
    while(!ui->widget->playerList.isEmpty())
        delete ui->widget->playerList.takeFirst();
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
        if(message.isEmpty()) break;
        QStringList info = message.split(" ", QString::SkipEmptyParts);
        if(info[0] == "NewPlayer:")
        {
            int idInfo = info[1].toInt();
            ui->widget->playerList.append(new Tank(idInfo));
            GLfloat xPosInfo = (GLfloat)info[2].toDouble();
            GLfloat yPosInfo = (GLfloat)info[3].toDouble();
            GLfloat rotInfo = (GLfloat)info[4].toDouble();
            GLfloat cannonRotInfo = (GLfloat)info[5].toDouble();
            ui->widget->playerList[idInfo]->setPos(xPosInfo , yPosInfo);
            ui->widget->playerList[idInfo]->setRotation(rotInfo);
            ui->widget->playerList[idInfo]->setCannonRotation(cannonRotInfo);
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
        else if(info[0] == "NewMissile:")
        {
            GLfloat xPosInfo = (GLfloat)info[1].toDouble();
            GLfloat yPosInfo = (GLfloat)info[2].toDouble();
            GLfloat angleInfo = (GLfloat)info[3].toDouble();
            ui->widget->missileList.append(new Missile(xPosInfo,yPosInfo,angleInfo));
            break;
        }
        else if(info[0] == "DeleteMissile:")
        {
            int idInfo = info[1].toInt();
            delete ui->widget->missileList.takeAt(idInfo);
        }
        else if(info[0] == "ClientDisconnected:")
        {
            int idInfo = info[1].toInt();
            delete ui->widget->playerList.takeAt(idInfo);
            if(playerID > idInfo)
            {
                playerID--;
                ui->widget->playerList[playerID]->id--;
            }
            break;
        }
        else
        {
            int idInfo = info[0].toInt();
            GLfloat xPosInfo = (GLfloat)info[1].toDouble();
            GLfloat yPosInfo = (GLfloat)info[2].toDouble();
            GLfloat rotInfo = (GLfloat)info[3].toDouble();
            GLfloat cannonRotInfo = (GLfloat)info[4].toDouble();
            ui->widget->playerList[idInfo]->setPos(xPosInfo , yPosInfo);
            ui->widget->playerList[idInfo]->setRotation(rotInfo);
            ui->widget->playerList[idInfo]->setCannonRotation(cannonRotInfo);
        }
    }
    ui->widget->updateGL();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
            keyUp = true;
            break;
        case Qt::Key_Down:
            keyDown = true;
            break;
        case Qt::Key_Left:
            keyLeft = true;
            break;
        case Qt::Key_Right:
            keyRight = true;
            break;
        case Qt::Key_E:
            keyE = true;
            break;
        case Qt::Key_Q:
            keyQ = true;
            break;
        case Qt::Key_Space:
        if(ui->widget->playerList[playerID]->hasAmmo() && ui->widget->playerList[playerID]->canShoot())
        {
            QTextStream out(socket);
            QString message = "NewMissile: " + QString::number(playerID) + " " +
                                               QString::number(ui->widget->playerList[playerID]->getXPos()) + " " +
                                               QString::number(ui->widget->playerList[playerID]->getYPos()) + " " +
                                               QString::number(ui->widget->playerList[playerID]->getCannonRotation() + ui->widget->playerList[playerID]->getRotation());
            out << message << endl;
            ui->widget->playerList[playerID]->takeAmmo(1);
            ui->widget->playerList[playerID]->canShoot(false);
            missileTimer.start();
        }
        return;
    }
    moved = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
            keyUp = false;
            break;
        case Qt::Key_Down:
            keyDown = false;
            break;
        case Qt::Key_Left:
            keyLeft = false;
            break;
        case Qt::Key_Right:
            keyRight = false;
            break;
        case Qt::Key_E:
            keyE = false;
            break;
        case Qt::Key_Q:
            keyQ = false;
            break;
    }
}

void MainWindow::movePlayer()
{
    if(keyUp)
    {
        if(ui->widget->playerList[playerID]->canMove(0.2, &ui->widget->playerList, 20, 20, -20, -20, 1.5))
            ui->widget->playerList[playerID]->move(0.3);
    }
    else if(keyDown)
    {
        if(ui->widget->playerList[playerID]->canMove(-0.1, &ui->widget->playerList, 20, 20, -20, -20, 1.5))
            ui->widget->playerList[playerID]->move(-0.2);
    }
    if(keyLeft)
        ui->widget->playerList[playerID]->rotate(-5);
    else if(keyRight)
        ui->widget->playerList[playerID]->rotate(5);
    if(keyE)
        ui->widget->playerList[playerID]->rotateCannon(-3);
    else if(keyQ)
        ui->widget->playerList[playerID]->rotateCannon(3);

    QTextStream out(socket);
    out << QString::number(ui->widget->playerList[playerID]->id)
           + " " + QString::number(ui->widget->playerList[playerID]->getXPos())
           + " " + QString::number(ui->widget->playerList[playerID]->getYPos())
           + " " + QString::number(ui->widget->playerList[playerID]->getRotation())
           + " " + QString::number(ui->widget->playerList[playerID]->getCannonRotation()) << endl;
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
    else
    {
        exit(0);
    }
}

void MainWindow::onTimer()
{
    if(moved)
    {
        movePlayer();
        moved = false;
        ui->widget->updateGL();
    }
    if(!ui->widget->missileList.isEmpty())
    {
        for(int i = ui->widget->missileList.size()-1; i >= 0; --i)
        {
            ui->widget->missileList[i]->move();
        }
        ui->widget->updateGL();
    }
}

void MainWindow::onMissileTimer()
{
    ui->widget->playerList[playerID]->canShoot(true);
}

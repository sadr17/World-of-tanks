#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSizePolicy qsp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    this->setSizePolicy(qsp);
    setCentralWidget(ui->widget);
    this->setFixedSize(1280,800);

    playerID = 0;
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(infoReceived()));
    connectBox();

    keyUp = keyDown = keyLeft = keyRight = keyE = keyQ = false;

    timerInterval = 15;

    timer.setSingleShot(false);
    timer.setInterval(timerInterval);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer.start();
    mt = 0;
    mtON = false;
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
    QString message = in.readLine();
    qDebug() << "Dostalem wiadomosc: " + message;
    if(message.isEmpty()) return;
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
        qDebug() << "Stworzylem nowy pocisk";
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
    }
    else if(info[0] == "PlayerKilled:")
    {
        int idInfo = info[1].toInt();
        GLfloat xPosInfo = (GLfloat)info[2].toDouble();
        GLfloat yPosInfo = (GLfloat)info[3].toDouble();
        GLfloat rotInfo = (GLfloat)info[4].toDouble();
        GLfloat cannonRotInfo = (GLfloat)info[5].toDouble();
        ui->widget->playerList[idInfo]->setPos(xPosInfo , yPosInfo);
        ui->widget->playerList[idInfo]->setRotation(rotInfo);
        ui->widget->playerList[idInfo]->setCannonRotation(cannonRotInfo);
    }
    else
    {
        int idInfo = info[0].toInt();
        qDebug() << "Przesuwam gracza nr: " + info[0];
        GLfloat xPosInfo = (GLfloat)info[1].toDouble();
        GLfloat yPosInfo = (GLfloat)info[2].toDouble();
        GLfloat rotInfo = (GLfloat)info[3].toDouble();
        GLfloat cannonRotInfo = (GLfloat)info[4].toDouble();
        ui->widget->playerList[idInfo]->setPos(xPosInfo , yPosInfo);
        ui->widget->playerList[idInfo]->setRotation(rotInfo);
        ui->widget->playerList[idInfo]->setCannonRotation(cannonRotInfo);
    }
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
                ui->widget->ammoHud = ui->widget->playerList[playerID]->ammoText();
                ui->widget->ammoProgress = 0;
                ui->widget->playerList[playerID]->canShoot(false);
                mtON = true;
            }
            break;
    }
    QMainWindow::keyPressEvent(event);
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
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::movePlayer()
{
    bool moving = false;
    if(keyUp)
    {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(0.18, &ui->widget->playerList, mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
            ui->widget->playerList[playerID]->move(0.18);
        moving = true;
    }
    else if(keyDown)
    {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(-0.1, &ui->widget->playerList, mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
            ui->widget->playerList[playerID]->move(-0.1);
        moving = true;
    }
    if(keyLeft)
    {
        ui->widget->playerList[playerID]->rotate(-5);
        moving = true;
    }
    else if(keyRight)
    {
        ui->widget->playerList[playerID]->rotate(5);
        moving = true;
    }
    if(keyE)
    {
        ui->widget->playerList[playerID]->rotateCannon(3);
        moving = true;
    }
    else if(keyQ)
    {
        ui->widget->playerList[playerID]->rotateCannon(-3);
        moving = true;
    }

    if(!moving) return;

    QTextStream out(socket);
    QString message = QString::number(ui->widget->playerList[playerID]->id)
            + " " + QString::number(ui->widget->playerList[playerID]->getXPos())
            + " " + QString::number(ui->widget->playerList[playerID]->getYPos())
            + " " + QString::number(ui->widget->playerList[playerID]->getRotation())
            + " " + QString::number(ui->widget->playerList[playerID]->getCannonRotation());
    out << message << endl;
    qDebug() << "Wysylam wiadomosc: " + message;
}

void MainWindow::connectBox()
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
    if(mtON)
    {
        if(mt >= 2000)
        {
            ui->widget->playerList[playerID]->canShoot(true);
            mtON = false;
            mt = 0;
            ui->widget->ammoProgress = 1.0;
        }
        else
        {
            mt += timerInterval;
            ui->widget->ammoProgress += ((GLfloat)timerInterval)/1000/2;
        }
    }

    if(!ui->widget->missileList.isEmpty())
    {
        for(int i = ui->widget->missileList.size()-1; i >= 0; --i)
        {
            ui->widget->missileList[i]->move();
            qDebug() << "Przesuwam pocisk " + QString::number(i);
        }
    }
    movePlayer();
    ui->widget->updateGL();
}

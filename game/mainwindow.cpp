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
    this->setFixedSize(1152,720);

    playerID = 0;
//    connectBox();

    keyUp = keyDown = keyLeft = keyRight = keyE = keyQ = keySpace = false;

    timerInterval = 15;

    timer.setSingleShot(false);
    timer.setInterval(timerInterval);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer.start();
    mt = 0;
    mtON = false;
    roundTimerEnabled = false;

    this->setupMap();
    this->setDefaultPos();
    this->setupPlayers();
}

MainWindow::~MainWindow()
{
    while(!ui->widget->playerList.isEmpty())
        delete ui->widget->playerList.takeFirst();
    delete ui;
}

void MainWindow::setupMap()
{
    map.append(new Obstacle(0, 0, 3.5));
    map.append(new Obstacle(-10, -2, 3.5));
    map.append(new Obstacle(10, 3, 3.5));
    map.append(new Obstacle(15, 0, 3.5));
    map.append(new Obstacle(-3, 1, 3.5));

    ui->widget->map.append(this->map);
}

void MainWindow::setupPlayers()
{
    int newID = ui->widget->playerList.size();

    // Enable game timer on first player join
    if(newID == 0) roundTimerEnabled = true;

    playerID = newID;
    ui->widget->playerID = playerID;
    ui->widget->playerList.append(new Tank(newID, defaultPosTab[newID][0], defaultPosTab[newID][1], defaultPosTab[newID][2]));
    ui->widget->playerList[playerID]->setColor(82, 122, 22);
    ui->widget->playerList[playerID]->setCannonColor(43, 69, 5);
    ui->widget->scoreboard.append(new Score(playerID));

    ui->widget->gameStatus = true;

//    int idInfo = info[1].toInt();
//    GLfloat xPosInfo = (GLfloat)info[2].toDouble();
//    GLfloat yPosInfo = (GLfloat)info[3].toDouble();
//    GLfloat rotInfo = (GLfloat)info[4].toDouble();
//    GLfloat cannonRotInfo = (GLfloat)info[5].toDouble();
//    int killsInfo = info[6].toInt();
//    int deathsInfo = info[7].toInt();
//    ui->widget->playerList.append(new Tank(idInfo, xPosInfo, yPosInfo, rotInfo));
//    ui->widget->playerList[idInfo]->setCannonRotation(cannonRotInfo);
//    ui->widget->scoreboard.append(new Score(idInfo, killsInfo, deathsInfo));
}

void MainWindow::setDefaultPos()
{
    defaultPosTab[0][0] = -15; // xPos
    defaultPosTab[0][1] = 15; // yPos
    defaultPosTab[0][2] = 140; // rot

    defaultPosTab[1][0] = 15;
    defaultPosTab[1][1] = 15;
    defaultPosTab[1][2] = 210;

    defaultPosTab[2][0] = 15;
    defaultPosTab[2][1] = -15;
    defaultPosTab[2][2] = 320;

    defaultPosTab[3][0] = -15;
    defaultPosTab[3][1] = -15;
    defaultPosTab[3][2] = 50;
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
            keySpace = true;
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
        case Qt::Key_Space:
            keySpace = false;
            break;
    }
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::updateGame(QString &data)
{
    QStringList info = data.split(" ", QString::SkipEmptyParts);
    if(info[0] == "NewPlayer:")
    {
        int idInfo = info[1].toInt();
        GLfloat xPosInfo = (GLfloat)info[2].toDouble();
        GLfloat yPosInfo = (GLfloat)info[3].toDouble();
        GLfloat rotInfo = (GLfloat)info[4].toDouble();
        GLfloat cannonRotInfo = (GLfloat)info[5].toDouble();
        ui->widget->playerList.append(new Tank(idInfo, xPosInfo, yPosInfo, rotInfo));
        ui->widget->playerList[idInfo]->setCannonRotation(cannonRotInfo);
        ui->widget->scoreboard.append(new Score(idInfo));
    }
//    else if(info[0] == "SetPlayer:")
//    {
//        int idInfo = info[1].toInt();
//        GLfloat xPosInfo = (GLfloat)info[2].toDouble();
//        GLfloat yPosInfo = (GLfloat)info[3].toDouble();
//        GLfloat rotInfo = (GLfloat)info[4].toDouble();
//        GLfloat cannonRotInfo = (GLfloat)info[5].toDouble();
//        int killsInfo = info[6].toInt();
//        int deathsInfo = info[7].toInt();
//        ui->widget->playerList.append(new Tank(idInfo, xPosInfo, yPosInfo, rotInfo));
//        ui->widget->playerList[idInfo]->setCannonRotation(cannonRotInfo);
//        ui->widget->scoreboard.append(new Score(idInfo, killsInfo, deathsInfo));
//    }
//    else if(info[0] == "IDMessage:")
//    {
//        int idInfo = info[1].toInt();
//        GLfloat xPosInfo = (GLfloat)info[2].toDouble();
//        GLfloat yPosInfo = (GLfloat)info[3].toDouble();
//        GLfloat rotInfo = (GLfloat)info[4].toDouble();
//        playerID = idInfo;
//        ui->widget->playerID = idInfo;
//        ui->widget->playerList.append(new Tank(idInfo, xPosInfo, yPosInfo, rotInfo));
//        ui->widget->playerList[playerID]->setColor(82, 122, 22);
//        ui->widget->playerList[playerID]->setCannonColor(43, 69, 5);
//        ui->widget->scoreboard.append(new Score(playerID));
//    }
    else if(info[0] == "DeleteMissile:")
    {
        int idInfo = info[1].toInt();
        delete ui->widget->missileList.takeAt(idInfo);
    }
    else if(info[0] == "ClientDisconnected:")
    {
        int idInfo = info[1].toInt();
        delete ui->widget->playerList.takeAt(idInfo);
        delete ui->widget->scoreboard.takeAt(idInfo);
        if(playerID > idInfo)
        {
            playerID--;
            ui->widget->playerID--;
            ui->widget->playerList[playerID]->id--;
            ui->widget->scoreboard[playerID]->tankID--;
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
    else if(info[0] == "UpdateScoreboard:")
    {
        int idInfo = info[1].toInt();
        int killsInfo = info[2].toInt();
        int deathsInfo = info[3].toInt();
        ui->widget->scoreboard[idInfo]->setKills(killsInfo);
        ui->widget->scoreboard[idInfo]->setDeaths(deathsInfo);
    }
    else if(info[0] == "Time:")
    {
        int timeInfo = info[1].toInt();
        ui->widget->gameTimer = timeInfo;
        roundTimerEnabled = true;
        if(timeInfo > 0)
        {
            ui->widget->gameStatus = true;
        }
    }
    else if(info[0] == "StartRound")
    {
        for(int i = 0; i < ui->widget->scoreboard.size(); ++i)
        {
            ui->widget->scoreboard[i]->reset();
        }
        ui->widget->gameStatus = true;
    }
    else if(info[0] == "RoundFinished")
    {
        roundTimer = 0;
        roundTimerEnabled = false;
        ui->widget->gameStatus = false;
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

void MainWindow::fire()
{
    //TO-DO: add hit() method to missle
    int tankID = playerID;
    GLfloat xPosInfo = ui->widget->playerList[playerID]->getXPos();
    GLfloat yPosInfo = ui->widget->playerList[playerID]->getYPos();
    GLfloat angleInfo = ui->widget->playerList[playerID]->getCannonRotation() + ui->widget->playerList[playerID]->getRotation();
    ui->widget->missileList.append(new Missile(xPosInfo,yPosInfo,angleInfo));
}

void MainWindow::movePlayer()
{
    //if(!roundTimerEnabled) return;
    bool moving = false;
    bool shooting = false;
    if(keyUp)
    {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(0.10, &ui->widget->playerList, &ui->widget->map, mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
            ui->widget->playerList[playerID]->move(0.10);
        moving = true;
    }
    else if(keyDown)
    {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(-0.05, &ui->widget->playerList, &ui->widget->map, mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
            ui->widget->playerList[playerID]->move(-0.05);
        moving = true;
    }
    if(keyLeft)
    {
        ui->widget->playerList[playerID]->rotate(-2);
        moving = true;
    }
    else if(keyRight)
    {
        ui->widget->playerList[playerID]->rotate(2);
        moving = true;
    }
    if(keyE)
    {
        ui->widget->playerList[playerID]->rotateCannon(1);
        moving = true;
    }
    else if(keyQ)
    {
        ui->widget->playerList[playerID]->rotateCannon(-1);
        moving = true;
    }
    if(keySpace)
    {
        if(ui->widget->playerList[playerID]->canShoot())
        {
            this->fire();
            ui->widget->ammoProgress = 0;
            ui->widget->playerList[playerID]->canShoot(false);
            mtON = true;
            shooting = true;
        }
    }

    if(!moving && !shooting) return;
}

//TO-DO: Change connection to node
void MainWindow::connectBox()
{
    bool ok;
    QString connectionData = QInputDialog::getText(this, tr("Enter node address"), tr("Enter address of node"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok && !connectionData.isEmpty())
    {
        QStringList data = connectionData.split(":", QString::SkipEmptyParts);
    }
    else
    {
        exit(0);
    }
}

void MainWindow::onTimer()
{
    if(roundTimerEnabled)
    {
        ui->widget->gameTimer -= timerInterval;
        if(ui->widget->gameTimer <= 0)
        {
            ui->widget->gameTimer = 0;
            roundTimerEnabled = false;
        }
    }

    movePlayer();

    if(mtON)
    {
        if(mt >= 1000)
        {
            ui->widget->playerList[playerID]->canShoot(true);
            mtON = false;
            mt = 0;
            ui->widget->ammoProgress = 1.0;
        }
        else
        {
            mt += timerInterval;
            ui->widget->ammoProgress += ((GLfloat)timerInterval)/500/2;
        }
    }

    if(!ui->widget->missileList.isEmpty())
    {
        for(int i = ui->widget->missileList.size()-1; i >= 0; --i)
        {
            ui->widget->missileList[i]->move();
        }
    }
    ui->widget->updateGL();
}

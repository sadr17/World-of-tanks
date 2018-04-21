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
    playerID = 0;

    connectBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMap()
{
    map.append(new Obstacle(-18, -8, 5));
    map.append(new Obstacle(-8, -2, 2));
    map.append(new Obstacle(14, -5, 4.8));
    map.append(new Obstacle(11, 13, 3));
    map.append(new Obstacle(-9, 9.9, 3.9));

    ui->widget->map.append(this->map);
}

void MainWindow::initPlayer()
{
    auto position = this->getRandomPos();
    ui->widget->playerID = playerID;
    ui->widget->playerList[playerID] = new Tank(playerID,
            position.value(QString("xPos")),
            position.value(QString("yPos")),
            position.value(QString("rot")),
            this);
    ui->widget->playerList[playerID]->setColor(82, 122, 22);
    ui->widget->playerList[playerID]->setCannonColor(43, 69, 5);
    ui->widget->scoreboard[playerID] = new Score(playerID, this);

    ui->widget->gameStatus = true;
}

void MainWindow::initTimer()
{
    timerInterval = 15;

    timer.setSingleShot(false);
    timer.setInterval(timerInterval);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer.start();
    mt = 0;
    mtON = false;
}

QMap<QString, double> MainWindow::getRandomPos()
{
    int randomId = this->playerID % 4;

    int xHigh, xLow, yHigh, yLow;
    int rHigh = 320;
    int rLow = 50;
    switch(randomId)
    {
    case 0:
        xHigh = 0;
        xLow = -15;
        yHigh = 15;
        yLow = 0;
        break;
    case 1:
        xHigh = 15;
        xLow = 0;
        yHigh = 15;
        yLow = 0;
        break;
    case 2:
        xHigh = 15;
        xLow = 0;
        yHigh = 0;
        yLow = -15;
        break;
    case 3:
        xHigh = 0;
        xLow = -15;
        yHigh = 0;
        yLow = -15;
        break;
    }

    qsrand(QDateTime::currentDateTime().time().msec());
    QString xPos("xPos"), yPos("yPos"), rot("rot");
    QMap<QString, double> result;
    result[xPos] = qrand() % ((xHigh + 1) - xLow) + xLow;;
    result[yPos] = qrand() % ((yHigh + 1) - yLow) + yLow;
    result[rot] = qrand() % ((rHigh + 1) - rLow) + rLow;
    return result;
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

void MainWindow::fire()
{
    GLfloat xPosInfo = ui->widget->playerList[playerID]->getXPos();
    GLfloat yPosInfo = ui->widget->playerList[playerID]->getYPos();
    GLfloat angleInfo = ui->widget->playerList[playerID]->getCannonRotation() + ui->widget->playerList[playerID]->getRotation();
    Missile * missile = new Missile(playerID, xPosInfo,yPosInfo,angleInfo);
    ui->widget->missileList.append(missile);
    this->game->fire(missile);
}

void MainWindow::movePlayer()
{
    foreach(auto key, this->keys.keys()) {
     if (!this->keys[key])
         continue;

     if(key == Qt::Key_Up)
     {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(0.10, ui->widget->playerList.values(), mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
            ui->widget->playerList[playerID]->move(0.10);
     }
     else if(key == Qt::Key_Down)
     {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(-0.05, ui->widget->playerList.values(), mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
            ui->widget->playerList[playerID]->move(-0.05);
     }
     if(key == Qt::Key_Left)
     {
        ui->widget->playerList[playerID]->rotate(-2);
     }
     else if(key == Qt::Key_Right)
     {
        ui->widget->playerList[playerID]->rotate(2);
     }
     if(key == Qt::Key_E)
     {
        ui->widget->playerList[playerID]->rotateCannon(1);
     }
     else if(key == Qt::Key_Q)
     {
        ui->widget->playerList[playerID]->rotateCannon(-1);
     }
     if(key == Qt::Key_Space)
     {
        if(ui->widget->playerList[playerID]->canShoot())
        {
            this->fire();
            ui->widget->ammoProgress = 0;
            ui->widget->playerList[playerID]->canShoot(false);
            mtON = true;
        }
     }
    }

    this->game->update(ui->widget->playerList[playerID]);
}

void MainWindow::connectBox()
{
    bool ok;
    QString connectionData = QInputDialog::getText(this,
                                                   tr("Enter node address"),
                                                   tr("Enter address of node"),
                                                   QLineEdit::Normal,
                                                   Utils::getCurrentIpAddress(), &ok);
    this->game = new GameConnector(this);
    if (ok && !connectionData.isEmpty())
    {
        this->game->registerSelf(connectionData);
        connect(this->game, &GameConnector::registered, [this](){
            this->playerID = this->game->currentPlayerId();

            this->setupMap();
            this->initPlayer();
            this->initTimer();

            connect(this->game, SIGNAL(updateReplica(int, float,float,float,float)), this, SLOT(updateReplica(int, float, float, float, float)));
            connect(this->game, SIGNAL(fireReplica(int,float,float,float)), SLOT(fireReplica(int, float, float, float)));
            connect(this->game, SIGNAL(killedReplica(int, int)), SLOT(killedReplica(int, int)));
            connect(this->game, &GameConnector::replicaAdded, [this]() {
                this->game->update(ui->widget->playerList[playerID]);
            });

            connect(this->game, &GameConnector::replicaRemoved, [this](int id){
               if (ui->widget->playerList.contains(id))
                    ui->widget->playerList.remove(id);

               if (ui->widget->scoreboard.contains(id))
                    ui->widget->scoreboard.remove(id);
            });
        });
     }
    else
    {
        exit(0);
    }
}

void MainWindow::updateReplica(int id, float x, float y, float an, float cAn)
{
    if (!ui->widget->playerList.contains(id))
    {
      ui->widget->playerList[id] = new Tank(id, x, y, an, this);
      ui->widget->scoreboard[id] = new Score(id, this);
    }

    ui->widget->playerList[id]->setPos(x, y);
    ui->widget->playerList[id]->setRotation(an);
    ui->widget->playerList[id]->setCannonRotation(cAn);
}

void MainWindow::fireReplica(int id, float x, float y, float an)
{
    ui->widget->missileList.append(new Missile(id, x, y, an, this));
}

void MainWindow::killedReplica(int id, int hitId)
{
    int death = ui->widget->scoreboard[id]->getDeaths();
    ui->widget->scoreboard[id]->setDeaths(++death);

    int kills = ui->widget->scoreboard[hitId]->getKills();
    ui->widget->scoreboard[hitId]->setKills(++kills);
}

void MainWindow::handleMissiles()
{
    if(!ui->widget->missileList.isEmpty())
    {
        for(int i = ui->widget->missileList.size()-1; i >= 0; --i)
        {
            int tankID = ui->widget->missileList[i]->hit(ui->widget->playerList.values());
            if (tankID >= 0 && tankID == this->playerID)
            {
                int tankHitId = ui->widget->missileList[i]->tankID;
                auto position = this->getRandomPos();

                ui->widget->playerList[tankID]->setRotation(position.value(QString("rot")));
                ui->widget->playerList[tankID]->setCannonRotation(0);
                ui->widget->playerList[tankID]->setPos(position.value(QString("xPos")), position.value(QString("yPos")));

                int death = ui->widget->scoreboard[tankID]->getDeaths();
                ui->widget->scoreboard[tankID]->setDeaths(++death);

                int kills = ui->widget->scoreboard[tankHitId]->getKills();
                ui->widget->scoreboard[tankHitId]->setKills(++kills);

                this->game->kill(tankHitId, ui->widget->playerList[tankID]);
            }
            if(ui->widget->missileList[i]->canMove(18, 32, -16.8, -32, map))
            {
               ui->widget->missileList[i]->move();
            }
            else
            {
               ui->widget->missileList.removeAt(i);
            }

        }
    }
}

void MainWindow::handleAmmoProgress()
{
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
}

void MainWindow::onTimer()
{
    movePlayer();
    handleAmmoProgress();
    handleMissiles();
    ui->widget->updateGL();
}

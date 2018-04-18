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

    keyUp = keyDown = keyLeft = keyRight = keyE = keyQ = keySpace = false;
    playerID = 0;

    connectBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMap()
{
    map.append(new Obstacle(-15, -6, 5));
    map.append(new Obstacle(-10, -2, 2));
    map.append(new Obstacle(11, 3, 2.8));
    map.append(new Obstacle(15, 0, 3));
    map.append(new Obstacle(-4, 0.9, 3.9));

    ui->widget->map.append(this->map);
}

void MainWindow::initPlayer()
{
    qsrand(this->playerID);
    int randomId = qrand() % 4;
    ui->widget->playerID = playerID;
    ui->widget->playerList[playerID] = new Tank(playerID,
            defaultPosTab[randomId][0],
            defaultPosTab[randomId][1],
            defaultPosTab[randomId][2],
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
    bool moving = false;
    bool shooting = false;
    if(keyUp)
    {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(0.10, ui->widget->playerList.values(), &ui->widget->map, mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
            ui->widget->playerList[playerID]->move(0.10);
        moving = true;
    }
    else if(keyDown)
    {
        GLint mapWidth = (ui->widget->mapWidth)/2;
        GLint mapHeight = (ui->widget->mapHeight)/2;
        if(ui->widget->playerList[playerID]->canMove(-0.05, ui->widget->playerList.values(), &ui->widget->map, mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
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
    this->game = new Game(this);
    if (ok && !connectionData.isEmpty())
    {
        this->game->registerSelf(connectionData);
        connect(this->game, &Game::registered, [this](){
            this->playerID = this->game->currentPlayerId();

            this->setupMap();
            this->setDefaultPos();
            this->initPlayer();
            this->initTimer();

            connect(this->game, SIGNAL(updateReplica(int, float,float,float,float)), this, SLOT(updateReplica(int, float, float, float, float)));
            connect(this->game, SIGNAL(fireReplica(int,float,float,float)), SLOT(fireReplica(int, float, float, float)));
            connect(this->game, SIGNAL(killedReplica(int, int)), SLOT(killedReplica(int, int)));
            connect(this->game, &Game::replicaAdded, [this]() {
                this->game->update(ui->widget->playerList[playerID]);
            });

            connect(this->game, &Game::replicaRemoved, [this](int id){
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
                qsrand(this->playerID);
                int randomId = qrand() % 4;

                ui->widget->playerList[tankID]->setRotation(defaultPosTab[randomId][3]);
                ui->widget->playerList[tankID]->setCannonRotation(0);
                ui->widget->playerList[tankID]->setPos(defaultPosTab[randomId][0], defaultPosTab[randomId][1]);

                int death = ui->widget->scoreboard[tankID]->getDeaths();
                ui->widget->scoreboard[tankID]->setDeaths(++death);

                int kills = ui->widget->scoreboard[tankHitId]->getKills();
                ui->widget->scoreboard[tankHitId]->setKills(++kills);

                this->game->meKilled(tankHitId, ui->widget->playerList[tankID]);
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

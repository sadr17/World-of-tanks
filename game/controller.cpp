#include "controller.h"
#include <QDateTime>

Controller::Controller(QObject *parent) :
    QObject(parent)
{

}

void Controller::connectToRegistry(const QString &registry)
{
    this->game = new GameConnector(this);
    this->game->registerSelf(registry);
    connect(this->game, &GameConnector::registered, [this](){
        this->playerID = this->game->currentPlayerId();

        this->initMap();
        this->initPlayer();
        this->initTimer();

        connect(this->game, SIGNAL(updateReplica(int, float,float,float,float)), this, SLOT(updateReplica(int, float, float, float, float)));
        connect(this->game, SIGNAL(fireReplica(int)), SLOT(fireReplica(int)));
        connect(this->game, SIGNAL(killedReplica(int, int)), SLOT(killedReplica(int, int)));
        connect(this->game, &GameConnector::replicaAdded, [this]() {
            this->game->update(this->players[playerID]);
        });

        connect(this->game, &GameConnector::replicaRemoved, [this](int id){
           if (this->players.contains(id)) {
               auto player = this->players.take(id);
               emit mapObjectRemoved(player);
           }

           if (this->scoreboard.contains(id))
                this->scoreboard.remove(id);
        });
    });
}

void Controller::handleKey(Qt::Key key)
{
    if(key == Qt::Key_Up)
    {
       GLint mapWidth = this->mapWidth / 2;
       GLint mapHeight = this->mapHeight / 2;
       if(this->players[playerID]->canMove(0.10, this->players.values(), mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
           this->players[playerID]->move(0.10);
    }
    else if(key == Qt::Key_Down)
    {
        GLint mapWidth = this->mapWidth / 2;
        GLint mapHeight = this->mapHeight / 2;
       if(this->players[playerID]->canMove(-0.05, this->players.values(), mapHeight, mapWidth, -mapHeight+1.2, -mapWidth, 1.5))
           this->players[playerID]->move(-0.05);
    }
    if(key == Qt::Key_Left)
    {
       this->players[playerID]->rotate(-2);
    }
    else if(key == Qt::Key_Right)
    {
       this->players[playerID]->rotate(2);
    }
    if(key == Qt::Key_E)
    {
       this->players[playerID]->rotateCannon(1);
    }
    else if(key == Qt::Key_Q)
    {
       this->players[playerID]->rotateCannon(-1);
    }
    if(key == Qt::Key_Space)
    {
       if(this->players[playerID]->canShoot())
       {
           this->fire();
           this->ammoProgress = 0;
           this->players[playerID]->canShoot(false);
           mtON = true;
       }
    }

    this->game->update(players[playerID]);
}

void Controller::setMapSize(float width, float height)
{
    this->mapWidth = width;
    this->mapHeight = height;
}

void Controller::initMap()
{
    map.append(new Obstacle(-19, -5, 5, this));
    map.append(new Obstacle(-2, -1, 2, this));
    map.append(new Obstacle(14, -5, 3.8, this));
    map.append(new Obstacle(11, 13, 3, this));
    map.append(new Obstacle(-9, 9.9, 3.9, this));

    foreach (auto obs, map)
        emit mapObjectAdded(obs);
}

void Controller::initPlayer()
{
    auto position = this->getRandomPos();

    players[playerID] = new Tank(playerID,
            position.value(QString("xPos")),
            position.value(QString("yPos")),
            position.value(QString("rot")),
            this);
    players[playerID]->setColor(82, 122, 22);
    players[playerID]->setCannonColor(43, 69, 5);
    this->scoreboard[playerID] = new Score(playerID, this);

    emit mapObjectAdded(players[playerID]);
}

void Controller::initTimer()
{
    timerInterval = 15;

    timer.setSingleShot(false);
    timer.setInterval(timerInterval);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer.start();
    mt = 0;
    mtON = false;
}

QMap<QString, double> Controller::getRandomPos()
{
    int randomId = this->playerID % 4;

    int xHigh, xLow, yHigh, yLow;
    int rHigh = 320;
    int rLow = 50;
    switch(randomId)
    {
    case 0:
        xHigh = -18;
        xLow = -mapWidth / 2.2;
        yHigh = 15;
        yLow = 0;
        break;
    case 1:
        xHigh = mapWidth / 2.2;
        xLow = 18;
        yHigh = mapHeight / 2.2;
        yLow = 5;
        break;
    case 2:
        xHigh = mapWidth / 2.2;
        xLow = 19;
        yHigh = -4.5;
        yLow = -mapHeight / 2.3;
        break;
    case 3:
        xHigh = -10;
        xLow = -mapWidth / 2.2;
        yHigh = -11;
        yLow = -mapHeight / 2.3;
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

void Controller::updateReplica(int id, float x, float y, float an, float cAn)
{
    if (!this->players.contains(id))
    {
      this->players[id] = new Tank(id, x, y, an, this);
      this->scoreboard[id] = new Score(id, this);
      emit mapObjectAdded(this->players[id]);
    }

    this->players[id]->setPos(x, y);
    this->players[id]->setAngle(an);
    this->players[id]->setCannonRotation(cAn);
}

void Controller::fireReplica(int id)
{
    Missile * missile = new Missile(this->players[id]);
    this->missiles.append(missile);
    emit mapObjectAdded(missile);
}

void Controller::killedReplica(int id, int hitId)
{
    int death = this->scoreboard[id]->getDeaths();
    this->scoreboard[id]->setDeaths(++death);

    int kills = this->scoreboard[hitId]->getKills();
    this->scoreboard[hitId]->setKills(++kills);
}

void Controller::handleMissiles()
{
        for(int i = this->missiles.size()-1; i >= 0; --i)
        {
            int tankID = this->missiles[i]->hit(this->players.values());
            if (tankID >= 0 && tankID == this->playerID)
            {
                int tankHitId = this->missiles[i]->getTankId();
                auto position = this->getRandomPos();

                this->players[tankID]->setAngle(position.value(QString("rot")));
                this->players[tankID]->setCannonRotation(0);
                this->players[tankID]->setPos(position.value(QString("xPos")), position.value(QString("yPos")));

                int death = this->scoreboard[tankID]->getDeaths();
                this->scoreboard[tankID]->setDeaths(++death);

                int kills = this->scoreboard[tankHitId]->getKills();
                this->scoreboard[tankHitId]->setKills(++kills);

                this->game->kill(tankHitId, this->players[tankID]);
            }
            if(this->missiles[i]->canMove(18, 32, -16.8, -32, map))
               this->missiles[i]->move();
            else
            {
               auto missile = this->missiles.takeAt(i);
               emit mapObjectRemoved(missile);
            }
        }
}

void Controller::handleAmmoProgress()
{
    if(mtON)
    {
        int timeToReload = 1000;
        if(mt >= timeToReload)
        {
            this->players[playerID]->canShoot(true);
            mtON = false;
            mt = 0;
            this->ammoProgress = 1.0;
        }
        else
        {
            mt += timerInterval;
            this->ammoProgress += (double)(timerInterval)/ 500 / 2;
        }
        emit ammoProgressChanged(this->ammoProgress);
    }
}

QMap<int, Score *> Controller::getScoreboard()
{
    return this->scoreboard;
}

int Controller::getPlayerId()
{
    return this->playerID;
}

void Controller::fire()
{
    Missile * missile = new Missile(this->players[playerID]);
    this->missiles.append(missile);
    this->game->fire();
    emit mapObjectAdded(missile);
}

void Controller::onTimer()
{
    handleAmmoProgress();
    handleMissiles();
    emit refresh();
}



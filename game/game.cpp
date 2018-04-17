#include "game.h"

Game::Game(QObject *parent) : QObject(parent)
{

}

void Game::registerSelf()
{
    this->src = new QRemoteObjectHost(QUrl(QString("tcp://%1:%2")
                               .arg(QString("127.0.0.1"))
                               .arg(QString("8080/test"))));

    this->self = new PlayerSource(this);
    this->src->enableRemoting(self);

    connect(this, SIGNAL(updatePosition(float,float,float,float)), this->self, SIGNAL(update(float,float,float,float)));
    connect(this, SIGNAL(fire(float,float,float)), this->self, SIGNAL(fire(float,float,float)));
    connect(this, SIGNAL(killed()), this->self, SIGNAL(killed()));
    qDebug()<<"Self registered";
}

void Game::addPlayer(QString &address)
{
    this->repNode = new QRemoteObjectNode(this);// create remote object node
    if (this->repNode->connectToNode(QUrl(QString("tcp://%1:%2")
                               .arg(address)
                               .arg(QString("8000/test"))))) // connect with remote host node

    {
    ptr.reset(repNode->acquire<PlayerReplica>()); // acquire replica of source from host node
    connect(ptr.data(), SIGNAL(update(float,float,float,float)), this, SIGNAL(updateReplica(float,float,float,float)));
    connect(ptr.data(), SIGNAL(fire(float,float,float)), this, SIGNAL(fireReplica(float,float,float)));
    connect(ptr.data(), SIGNAL(killed()), this, SIGNAL(killedReplica()));
    }

}

void Game::update(Tank *tank)
{
    qDebug()<<"update self position";
    emit updatePosition(tank->getXPos(), tank->getYPos(), tank->getRotation(), tank->getCannonRotation());
}

void Game::fire(Missile *missile)
{
    qDebug()<<"fire";
    emit fire(missile->getXPos(), missile->getYPos(), missile->getAngle());
}

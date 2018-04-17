#include "game.h"
#include "../common/utils.h"
#include "../build-game/rep_counter_replica.h"

Game::Game(QObject *parent) : QObject(parent)
{

}

void Game::registerSelf(QString registryAddress)
{
    QString ipAddress = Utils::getCurrentIpAddress();
    qsrand(QDateTime::currentDateTime().time().msec());
    int port = qrand() % 8000 + 8000;
    if (registryAddress.isEmpty())
        registryAddress = ipAddress;

    this->src = new QRemoteObjectHost(QUrl(QString("tcp://%1:%2")
                                           .arg(ipAddress)
                                           .arg(QString("%1/replica").arg(port))),
                                          QUrl(QString("tcp://%1:%2")
                                           .arg(registryAddress)
                                           .arg(QString("8080/registry"))), this);
    if(!this->src->waitForRegistry()) {
        qDebug()<<"Failed to connect to registry";
        return;
    }

    auto sources = this->src->registry()->sourceLocations();
    foreach(auto location, sources.keys()) {
        this->addReplica(location);
    }

    connect(this->src->registry(), &QRemoteObjectRegistry::remoteObjectAdded, [this](const QRemoteObjectSourceLocation &loc){
        this->addReplica(loc.first);
    });

    auto counter = this->src->acquire<CounterReplica>();
    connect(counter, &CounterReplica::initialized, [this, counter](){
        this->playerId = counter->counter();
        this->self = new PlayerSimpleSource(this);
        this->src->enableRemoting(this->self, QString("Player_%1").arg(this->playerId));

        connect(this, SIGNAL(updatePosition(float,float,float,float)), this->self, SIGNAL(update(float,float,float,float)));
        connect(this, SIGNAL(fire(float,float,float)), this->self, SIGNAL(fire(float,float,float)));
        connect(this, SIGNAL(killed()), this->self, SIGNAL(killed()));
        emit registered();
    });

}

void Game::addReplica(QString loc)
{
    if(loc == QString("Counter"))
        return;

    int id = loc.split("_").last().toInt();
    if(id == this->playerId)
        return;

    auto replica = this->src->acquire<PlayerReplica>(loc);
    if (replica) {
        connect(replica, &PlayerReplica::update, [this, id](float x,float y,float a,float ca) {
               emit updateReplica(id, x, y, a, ca);
        });
        connect(replica, &PlayerReplica::fire, [this, id](float x,float y,float a) {
               emit fireReplica(id, x, y, a);
        });
        connect(replica, &PlayerReplica::killed, [this, id]() {
               emit killedReplica(id);
        });
    }
}

void Game::update(Tank *tank)
{
    emit updatePosition(tank->getXPos(), tank->getYPos(), tank->getRotation(), tank->getCannonRotation());
}

void Game::fire(Missile *missile)
{
    emit fire(missile->getXPos(), missile->getYPos(), missile->getAngle());
}

void Game::meKilled(Tank * tank)
{
    emit killed();
    this->update(tank);
}

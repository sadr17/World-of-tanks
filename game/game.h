#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "tank.h"
#include "../build-game/rep_player_replica.h"
#include "../build-game/rep_player_source.h"
#include "missile.h"


/**
 * @brief The Game class
 *
 * 1. User starts the game
 * - create Player source and publish
 * - get list of players from the registration service
 */
class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);

    void registerSelf(QString registryAddress);
    void update(Tank *tank);
    void fire(Missile * missile);
    void meKilled(int hitId, Tank * tank);
    int currentPlayerId() {
        return this->playerId;
    }

signals:
    void updatePosition(float xPos, float yPos, float rotation, float cannonRotation);
    void fire(float xPos, float yPos, float angle);
    void killed(int hitId);

    void updateReplica(int id, float xPos, float yPos, float rotation, float cannonRotation);
    void fireReplica(int id, float xPos, float yPos, float angle);
    void killedReplica(int id, int hitId);

    void registered();

    void replicaAdded();
    void replicaRemoved(int id);

public slots:

private:
    int playerId;
    // source
    PlayerSimpleSource *self;
    QRemoteObjectHost * src;

    //replicas
    QRemoteObjectNode *repNode;
    QList<QSharedPointer<PlayerReplica>> replicas;

    void addReplica(QString loc);
};

#endif // GAME_H

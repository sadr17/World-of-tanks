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
 *
 * 2. On new player
 * - connect to the new player, get replica
 */
class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);

    void registerSelf();
    void addPlayer(QString &address);
    void update(Tank *tank);
    void fire(Missile * missile);

signals:
    void updatePosition(float xPos, float yPos, float rotation, float cannonRotation);
    void fire(float xPos, float yPos, float angle);
    void killed();

    void updateReplica(float xPos, float yPos, float rotation, float cannonRotation);
    void fireReplica(float xPos, float yPos, float angle);
    void killedReplica();

public slots:

private:
    // source
    PlayerSource *self;
    QRemoteObjectHost *src;

    //replicas
    QRemoteObjectNode *repNode;
    QSharedPointer<PlayerReplica> ptr;

};

#endif // GAME_H

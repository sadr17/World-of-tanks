#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "tank.h"

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

    void registerSelf(Tank *tank);
    void addPlayer(PlayerSource *player);

signals:

public slots:
};

#endif // GAME_H

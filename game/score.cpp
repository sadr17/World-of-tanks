#include "score.h"

Score::Score(int i, QObject * parent) :
    QObject(parent)
{
    tankID = i;
    kills = 0;
    deaths = 0;
}

Score::Score(int i, int k, int d, QObject * parent) :
    QObject(parent)
{
    tankID = i;
    kills = k;
    deaths = d;
}

void Score::setDeaths(int value)
{
    deaths = value;
}

void Score::setKills(int value)
{
    kills = value;
}

QString Score::toString()
{
    return QString::number(tankID + 1) + " [" + QString::number(kills) + "/" + QString::number(deaths) + "]";
}

void Score::reset()
{
    deaths = 0;
    kills = 0;
}

int Score::getKills()
{
    return kills;
}

int Score::getDeaths()
{
    return deaths;
}

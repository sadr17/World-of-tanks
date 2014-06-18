#include "score.h"

Score::Score()
{
    kills = tankID = deaths = 0;
}

Score::Score(int i)
{
    tankID = i;
    kills = 0;
    deaths = 0;
}

Score::Score(int i, int k, int d)
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
    return QString::number(tankID + 1) + " - " + QString::number(kills) + "/" + QString::number(deaths);
}

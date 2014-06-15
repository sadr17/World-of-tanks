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

void Score::addKill()
{
    kills++;
}

void Score::addDeath()
{
    deaths++;
}

void Score::setDeaths(int value)
{
    deaths = value;
}

void Score::setKills(int value)
{
    kills = value;
}

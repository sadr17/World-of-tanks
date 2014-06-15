#include "score.h"

Score::Score()
{
    kills = deaths = 0;
    tankID = 0;
}

Score::Score(int i)
{
    tankID = i;
    kills = deaths = 0;
}

void Score::addDeath()
{
    deaths++;
}

void Score::addKill()
{
    kills++;
}

void Score::setID(int i)
{
    tankID = i;
}

#include "score.h"

Score::Score()
{
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

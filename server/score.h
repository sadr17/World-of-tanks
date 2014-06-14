#ifndef SCORE_H
#define SCORE_H

#include "tank.h"

class Score : public Tank
{
public:
    Score();
    void addKill();
    void addDeath();
private:
    int kills;
    int deaths;
};

#endif // SCORE_H

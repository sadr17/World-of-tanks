#ifndef SCORE_H
#define SCORE_H

class Score
{
public:
    Score();
    void addKill();
    void addDeath();
    int getKills();
    int getDeaths();
    void reset();
private:
    int kills;
    int deaths;
};

#endif // SCORE_H

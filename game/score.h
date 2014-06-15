#ifndef SCORE_H
#define SCORE_H

class Score
{
public:
    Score();
    Score(int i);
    void addKill();
    void addDeath();
    void setDeaths(int value);
    void setKills(int value);
    int tankID;
private:
    int kills;
    int deaths;
};

#endif // SCORE_H

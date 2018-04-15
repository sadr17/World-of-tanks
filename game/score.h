#ifndef SCORE_H
#define SCORE_H

#include <QString>

class Score
{
public:
    Score();
    Score(int i);
    Score(int i, int k, int d);
    void setDeaths(int value);
    void setKills(int value);
    void reset();
    int getKills();
    int getDeaths();
    QString toString();
    int tankID;
private:
    int kills;
    int deaths;
};

#endif // SCORE_H

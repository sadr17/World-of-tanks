#ifndef SCORE_H
#define SCORE_H

#include <QString>
#include <QObject>

class Score : public QObject
{
    Q_OBJECT
public:
    explicit Score(int i, QObject * parent);
    Score(int i, int k, int d, QObject * parent);
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

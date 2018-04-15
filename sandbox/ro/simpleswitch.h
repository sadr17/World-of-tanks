#ifndef SIMPLESWITCH_H
#define SIMPLESWITCH_H

#include "../build-ro-Desktop-Debug/rep_simpleswitch_source.h"
#include <QTimer>

class SimpleSwitch : public SimpleSwitchSimpleSource
{
    Q_OBJECT
public:
    SimpleSwitch(QObject *parent = nullptr);
    ~SimpleSwitch();
    virtual void server_slot(bool clientState);
public Q_SLOTS:
    void timeout_slot();
private:
    QTimer *stateChangeTimer;
};

#endif

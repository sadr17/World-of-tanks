#include "client.h"

// constructor
Client::Client(QSharedPointer<SimpleSwitchReplica> ptr) :
    QObject(nullptr),reptr(ptr)
{
    initConnections();
    //We can connect to SimpleSwitchReplica Signals/Slots
    //directly because our Replica was generated by repc.
}

//destructor
Client::~Client()
{
}

void Client::initConnections()
{
        // initialize connections between signals and slots

       // connect source replica signal currStateChanged() with client's recSwitchState() slot to receive source's current state
        QObject::connect(reptr.data(), SIGNAL(currStateChanged(bool)), this, SLOT(recSwitchState_slot()));
       // connect client's echoSwitchState(..) signal with replica's server_slot(..) to echo back received state
        QObject::connect(this, SIGNAL(echoSwitchState(bool)),reptr.data(), SLOT(server_slot(bool)));
}

void Client::recSwitchState_slot()
{
    qDebug() << "Received source state "<<reptr.data()->currState();
    clientSwitchState = reptr.data()->currState();
    Q_EMIT echoSwitchState(clientSwitchState); // Emit signal to echo received state back to server
}

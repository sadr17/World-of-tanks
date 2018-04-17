#include <QCoreApplication>

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectsource.h>
#include "../build-ro2-Desktop-Debug/rep_simpleswitch_replica.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QRemoteObjectNode repNode; // create remote object node
    repNode.connectToNode(QUrl("local:test")); // connect with remote host node

    QSharedPointer<SimpleSwitchReplica> ptr;
    ptr.reset(repNode.acquire<SimpleSwitchReplica>()); // acquire replica of source from host node

    Client rswitch(ptr); // create client switch object and pass reference of replica to it

    return a.exec();
}

#include <QCoreApplication>
#include "simpleswitch.h"

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectsource.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QRemoteObjectHost src(QUrl("local:test"));

    SimpleSwitch srcSwitch; // create simple switch
    src.enableRemoting(&srcSwitch);
//    qDebug() << QUrl("localhost");


    return a.exec();
}

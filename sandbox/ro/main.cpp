#include <QCoreApplication>
#include "simpleswitch.h"

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectsource.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


//    QRemoteObjectHost src(QUrl("tcp://10.240.23.53:8000/test"));
    //QRemoteObjectRegistry reg;
    QRemoteObjectRegistryHost re(QUrl("tcp://127.0.0.1:8000/test"));

    SimpleSwitch srcSwitch; // create simple switch
  //  src.enableRemoting(&srcSwitch);
    re.enableRemoting(&srcSwitch);


//    qDebug() << QUrl("localhost");


    return a.exec();
}

#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QtNetwork>

class Utils {

  public:
    static QString getCurrentIpAddress() {
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for(int i = 0; i < ipAddressesList.size(); ++i)
        {
            if(ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
            {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }

        if(ipAddress.isEmpty())
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

        return ipAddress;
    }
};

#endif // UTILS_H

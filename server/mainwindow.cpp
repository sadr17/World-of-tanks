#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->logWindow);
    openSession();
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSession()
{
    server = new QTcpServer(this);
    if(!server->listen())
    {
        close();
        return;
    }
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
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    ui->logWindow->append("Server running on address: " + ipAddress + ":" + QString::number(server->serverPort()) + "\n");
}

void MainWindow::newConnection()
{
    QTcpSocket* newSocket = server->nextPendingConnection();
    if(newSocket)
    {
        clients.push_back(newSocket);
        connect(newSocket,SIGNAL(readyRead()),this,SLOT(newInfo()));
        connect(newSocket,SIGNAL(disconnected()),this,SLOT(clientDisconnect()));
        ui->logWindow->append("Client connected\n");

        QTextStream out(newSocket);
        out << "PlayersOnline: " + QString::number(playersList.size()) << endl;
        if(!playersList.isEmpty())
        {

            int listSize = playersList.size();
            playersList.append(new Tank(listSize));

            for(int i = 0; i < playersList.size(); ++i)
            {
                QString message = QString::number(playersList[i]->id) + " " +
                                  QString::number(playersList[i]->getXPos()) + " " +
                                  QString::number(playersList[i]->getYPos()) + " " +
                                  QString::number(playersList[i]->getRotation());
                ui->logWindow->append("Message about online players sent to client " + QString::number(listSize) + ": " + message);
                out << message << endl;
            }

            for(int i = 0; i < clients.size() - 1; ++i)
            {
                QTextStream clientsOut(clients[i]);
                QString message = "NewPlayer: " +
                                  QString::number(playersList[listSize]->id) + " " +
                                  QString::number(playersList[listSize]->getXPos()) + " " +
                                  QString::number(playersList[listSize]->getYPos()) + " " +
                                  QString::number(playersList[listSize]->getRotation());
                ui->logWindow->append("Message about new player sent to client " + QString::number(i) + ": " + message);
                clientsOut << message << endl;
            }
        }
        else
            playersList.append(new Tank(playersList.size()));
    }
}

void MainWindow::newInfo()
{
    for(int i = clients.size()-1; i >= 0; --i)
    {
        if(clients[i]->isReadable() && clients[i]->bytesAvailable() > 0)
        {
            QTextStream in(clients[i]);
            QString info = in.readLine();
            QStringList infoList = info.split(" ", QString::SkipEmptyParts);
            int idInfo = infoList[0].toInt();
            double xInfo = infoList[1].toDouble();
            double yInfo = infoList[2].toDouble();
            playersList[idInfo]->setPos(xInfo, yInfo);
            double rotInfo = infoList[3].toDouble();
            playersList[idInfo]->setRotation(rotInfo);
            ui->logWindow->append("Info received: " + info);
            for(int j = 0; j < clients.size(); ++j)
            {
                if(clients[j]->isWritable())
                {
                    QTextStream out(clients[j]);
                    for(int k = 0; k < playersList.size(); ++k)
                    {
                        QString message = QString::number(playersList[k]->id) + " " +
                                          QString::number(playersList[k]->getXPos()) + " " +
                                          QString::number(playersList[k]->getYPos()) + " " +
                                          QString::number(playersList[k]->getRotation());
                        ui->logWindow->append("Message to client " + QString::number(j) + ": " + message);
                        out << message << endl;
                    }
                }
            }
        }
    }
}

void MainWindow::clientDisconnect()
{
    for(int i = clients.size() - 1; i >= 0; --i)
    {
        if(clients[i]->state() == QAbstractSocket::UnconnectedState)
        {
            int disconnectedPlayerID = playersList[i]->id;
            playersList.takeAt(i);
            clients.takeAt(i);
            ui->logWindow->append("Disconnecting client: " + QString::number(i));
            if(!clients.isEmpty())
            {
                for(int j = 0; j >= clients.size(); --j)
                {
                    QTextStream clientOut(clients[j]);
                    clientOut << "ClientDisconnected: " + QString::number(disconnectedPlayerID) << endl;
                }
            }
        }
    }
}

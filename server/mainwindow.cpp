#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->logWindow);
    // Connection
    openSession();
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    // Game timer
    timerInterval = 17;
    gameTimer.setSingleShot(false);
    gameTimer.setInterval(timerInterval);
    connect(&gameTimer,SIGNAL(timeout()),this,SLOT(onTimer()));
    gameTimer.start();
    // Default position tab
    setDefaultPos();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimer()
{
    if(!missileList.isEmpty())
    {
        for(int i = missileList.size() - 1; i >= 0 ;--i)
        {
            int tankHitID = missileList[i]->hit(&playersList);
            if(tankHitID >= 0)
            {
                playersList[tankHitID]->setCannonRotation(0);
                playersList[tankHitID]->setRotation(0);
                playersList[tankHitID]->setPos(0, 0);
                delete missileList.takeAt(i);
                QString message = getPlayerInfo(tankHitID);
                QString missileMessage = "DeleteMissile: " + QString::number(i);
                for(int j = 0; j < clients.size(); ++j)
                {
                    QTextStream out(clients[j]);
                    out << message << endl;
                    ui->logWindow->append("Message send to client " + QString::number(j) + ": " + message);
                    out << missileMessage << endl;
                    ui->logWindow->append("Message send to client " + QString::number(j) + ": " + message);
                }
                continue;
            }
            if(missileList[i]->canMove(18, 32, -16.8, -32))
            {
                missileList[i]->move();
            }
            else
            {
                QString missileMessage = "DeleteMissile: " + QString::number(i);
                for(int j = 0 ; j < clients.size(); ++j)
                {
                    QTextStream out(clients[j]);
                    out << missileMessage << endl;
                }
                delete missileList.takeAt(i);
            }
        }
    }
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
        onConnectMessage(newSocket);
    }
}


void MainWindow::onConnectMessage(QTcpSocket *_socket)
{
    QTextStream out(_socket);
    out << "PlayersOnline: " + QString::number(playersList.size()) << endl;
    if(!playersList.isEmpty())
    {
        int listSize = playersList.size();
        playersList.append(new Tank(listSize));

        for(int i = 0; i < playersList.size(); ++i)
        {
            QString message = getPlayerInfo(i);
            out << message << endl;
        }

        QString message = "NewPlayer: " + getPlayerInfo(listSize);

        for(int i = 0; i < clients.size() - 1; ++i)
        {
            QTextStream clientsOut(clients[i]);
            clientsOut << message << endl;
        }
    }
    else
        playersList.append(new Tank(playersList.size()));
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
            int idInfo;
            if(infoList[0] == "NewMissile:")
            {
                int tankID = infoList[1].toInt();
                double missileXPos = infoList[2].toDouble();
                double missileYPos = infoList[3].toDouble();
                double missileDirection = infoList[4].toDouble();
                missileList.append(new Missile(tankID, missileXPos, missileYPos, missileDirection));
                for(int j = 0; j < clients.size(); ++j)
                {
                    if(clients[j]->isWritable())
                    {
                        QTextStream out(clients[j]);
                        for(int k = 0; k < missileList.size(); ++k)
                        {
                            QString message = "NewMissile: " + getMissileInfo(k);
                            out << message << endl;
                        }
                    }
                }
            }
            else
            {
                idInfo = infoList[0].toInt();
                double xInfo = infoList[1].toDouble();
                double yInfo = infoList[2].toDouble();
                playersList[idInfo]->setPos(xInfo, yInfo);
                double rotInfo = infoList[3].toDouble();
                playersList[idInfo]->setRotation(rotInfo);
                double cannonRotInfo = infoList[4].toDouble();
                playersList[idInfo]->setCannonRotation(cannonRotInfo);
            }
            for(int j = 0; j < clients.size(); ++j)
            {
                if(clients[j]->isWritable())
                {
                    QTextStream out(clients[j]);
                    for(int k = 0; k < playersList.size(); ++k)
                    {
                        QString message = getPlayerInfo(k);
                        out << message << endl;
                    }
                }
            }
        }
    }
}

QString MainWindow::getPlayerInfo(int id)
{
    return QString::number(playersList[id]->id) + " " +
           QString::number(playersList[id]->getXPos()) + " " +
           QString::number(playersList[id]->getYPos()) + " " +
           QString::number(playersList[id]->getRotation()) + " " +
           QString::number(playersList[id]->getCannonRotation());
}

QString MainWindow::getMissileInfo(int id)
{
    return QString::number(missileList[id]->getXPos()) + " " +
           QString::number(missileList[id]->getYPos()) + " " +
           QString::number(missileList[id]->getDirection());
}

void MainWindow::clientDisconnect()
{
    for(int i = clients.size() - 1; i >= 0; --i)
    {
        if(clients[i]->state() == QAbstractSocket::UnconnectedState)
        {
            int disconnectedPlayerID = playersList[i]->id;
            delete playersList.takeAt(i);
            delete clients.takeAt(i);
            ui->logWindow->append("Disconnecting client " + QString::number(i));
            if(!clients.isEmpty())
            {
                for(int j = clients.size() - 1; j >= 0; --j)
                {
                    if(clients[j]->isWritable())
                    {
                        if(playersList[j]->id > disconnectedPlayerID)
                        {
                            playersList[j]->id = playersList[j]->id - 1;
                        }
                        QTextStream clientOut(clients[j]);
                        clientOut << "ClientDisconnected: " + QString::number(disconnectedPlayerID) << endl;
                    }
                }
            }
            break;
        }
    }
}

void MainWindow::setDefaultPos()
{
    defaultPosTab[0][0] = -15; // xPos
    defaultPosTab[0][1] = 15; // yPos
    defaultPosTab[0][2] = 140; // rot

    defaultPosTab[1][0] = 15;
    defaultPosTab[1][1] = 15;
    defaultPosTab[1][2] = 210;

    defaultPosTab[2][0] = 15;
    defaultPosTab[2][1] = -15;
    defaultPosTab[2][2] = 320;

    defaultPosTab[3][0] = -15;
    defaultPosTab[3][1] = -15;
    defaultPosTab[3][2] = 50;
}

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
    roundTimer = 5*60*1000;
    roundTimerEnabled = false;
    // Default position tab
    setDefaultPos();
    setupMap();
    // Default game status
    roundEnd = false;
    roundEndTimer = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    while(!map.isEmpty())
        delete map.takeFirst();
    while(!playersList.isEmpty())
        delete playersList.takeFirst();
    while(!clients.isEmpty())
        delete clients.takeFirst();
    while(!scoreboard.isEmpty())
        delete scoreboard.takeFirst();
}

void MainWindow::onTimer()
{
    // Main game timer
    if(roundEnd)
    {
        roundEndTimer += timerInterval;
        if(roundEndTimer >= 10000)
        {
            roundTimer = 5*60*1000;
            resetGame();

            for(int j = 0; j < clients.size(); ++j)
            {
                QTextStream out(clients[j]);
                out << "StartRound" << endl;
                out << "Time: " + QString::number(roundTimer) << endl;
                for(int i = playersList.size() - 1; i >= 0; --i)
                {
                    out << getPlayerInfo(i) << endl;
                }
            }
            roundTimerEnabled = true;
            roundEnd = false;
            roundEndTimer = 0;
        }
    }
    if(roundTimerEnabled)
    {
        roundTimer -= timerInterval;
        if(roundTimer <= 0)
        {
            roundTimer = 0;
            roundTimerEnabled = false;
            for(int j = 0; j < clients.size(); ++j)
            {
                QTextStream out(clients[j]);
                out << "RoundFinished" << endl;
            }
            roundEnd = true;
        }
    }
    // Missiles
    if(!missileList.isEmpty())
    {
        for(int i = missileList.size() - 1; i >= 0 ;--i)
        {
            int tankHitID = missileList[i]->hit(&playersList);
            if(tankHitID >= 0)
            {
                int randomTab = rand()%4;
                playersList[tankHitID]->setCannonRotation(0);
                playersList[tankHitID]->setRotation(defaultPosTab[randomTab][2]);
                playersList[tankHitID]->setPos(defaultPosTab[randomTab][0], defaultPosTab[randomTab][1]);
                int killerID = missileList[i]->tankID;
                scoreboard[tankHitID]->addDeath();
                scoreboard[killerID]->addKill();
                delete missileList.takeAt(i);
                QString message = getPlayerInfo(tankHitID);
                QString missileMessage = "DeleteMissile: " + QString::number(i);
                QString killerMessage = "UpdateScoreboard: " + getScoreInfo(killerID);
                QString victimMessage = "UpdateScoreboard: " + getScoreInfo(tankHitID);
                for(int j = 0; j < clients.size(); ++j)
                {
                    QTextStream out(clients[j]);
                    out << missileMessage << endl;
                    out << message << endl;
                    out << killerMessage << endl;
                    out << victimMessage << endl;
                }
                continue;
            }
            if(missileList[i]->canMove(18, 32, -16.8, -32, &map))
            {
                missileList[i]->move();
            }
            else
            {
                delete missileList.takeAt(i);
                QString missileMessage = "DeleteMissile: " + QString::number(i);
                for(int j = 0 ; j < clients.size(); ++j)
                {
                    QTextStream out(clients[j]);
                    out << missileMessage << endl;
                }
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
    if(clients.size() >= 4) return;
    if(newSocket)
    {
        clients.push_back(newSocket);
        connect(newSocket,SIGNAL(readyRead()),this,SLOT(newInfo()));
        connect(newSocket,SIGNAL(disconnected()),this,SLOT(clientDisconnect()));
        ui->logWindow->append("Client connected");
        onConnectMessage(newSocket);
    }
}


void MainWindow::onConnectMessage(QTcpSocket *_socket)
{
    QTextStream out(_socket);

    if(!map.isEmpty())
    {
        for(int i = 0; i < map.size(); ++i)
        {
            QString message = "Map: " + map[i]->getData();
            out << message << endl;
        }
    }

    if(!playersList.isEmpty())
    {
        for(int i = 0; i < playersList.size(); i++)
        {
            QString setPlayerMessage = "SetPlayer: " + getPlayerInfo(i) + " " + getScoreInfo(i, true);
            out << setPlayerMessage << endl;
        }
    }

    int newID = playersList.size();

    // Enable game timer on first player join
    if(newID == 0) roundTimerEnabled = true;

    playersList.append(new Tank(newID, defaultPosTab[newID][0], defaultPosTab[newID][1], defaultPosTab[newID][2]));
    scoreboard.append(new Score());
    QString IDMessage = "IDMessage: " + getPlayerInfo(newID);
    out << IDMessage << endl;

    QString message = "NewPlayer: " + getPlayerInfo(newID);

    if(newID > 0)
    {
        for(int i = 0; i < clients.size() - 1; ++i)
        {
            QTextStream clientStream(clients[i]);
            clientStream << message << endl;
        }
    }

    QString timeMessage = "Time: " + QString::number(roundTimer);
    out << timeMessage << endl;
}


void MainWindow::newInfo()
{
    for(int i = clients.size()-1; i >= 0; --i)
    {
        if(clients[i]->isReadable() && clients[i]->bytesAvailable() > 0)
        {
            QTextStream in(clients[i]);
            QString info;
            do
            {
                info = in.readLine();
                if(info.isEmpty()) break;
                updateGame(info);
            }
            while (!info.isNull());
        }
    }
}

void MainWindow::updateGame(QString &data)
{
    QStringList infoList = data.split(" ", QString::SkipEmptyParts);
    int idInfo;
    if(infoList[0] == "NewMissile:")
    {
        ui->logWindow->append("New Missile");
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
                QString message = "NewMissile: " + getMissileInfo(missileList.size() - 1);
                out << message << endl;
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
                if(k == j) continue;
                QString message = getPlayerInfo(k);
                out << message << endl;
                qDebug() << "Wiadomosc do gracza [" + QString::number(j) + "]: " + message;
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

QString MainWindow::getScoreInfo(int id, bool shorten)
{
    QString returnString = "";
    if(!shorten) returnString += QString::number(id) + " ";
    returnString += QString::number(scoreboard[id]->getKills()) + " " +
                    QString::number(scoreboard[id]->getDeaths());
    return returnString;
}

void MainWindow::clientDisconnect()
{
    for(int i = clients.size() - 1; i >= 0; --i)
    {
        if(clients[i]->state() == QAbstractSocket::UnconnectedState)
        {
            int disconnectedPlayerID = playersList[i]->id;
            delete playersList.takeAt(i);
            clients.takeAt(i)->deleteLater();
            delete scoreboard.takeAt(i);
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
            else
            {
                roundTimerEnabled = false;
                roundTimer = 5*60*1000;
            }
            break;
        }
    }
}

void MainWindow::resetGame()
{
    for(int i = 0; i < scoreboard.size(); ++i)
    {
        scoreboard[i]->reset();
    }

    for(int i = playersList.size() - 1; i >= 0; --i)
    {
        playersList[i]->setPos(defaultPosTab[i][0], defaultPosTab[i][1]);
        playersList[i]->setRotation(defaultPosTab[i][2]);
        playersList[i]->setCannonRotation(0);
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

void MainWindow::setupMap()
{
    map.append(new Obstacle(0, 0, 3.5));
    map.append(new Obstacle(-10, -2, 3.5));
    map.append(new Obstacle(10, 3, 3.5));
    map.append(new Obstacle(15, 0, 3.5));
    map.append(new Obstacle(-3, 1, 3.5));
}

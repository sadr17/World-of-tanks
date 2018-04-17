#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../common/utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->logWindow);
    openSession();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSession()
{
    auto ipAddress = Utils::getCurrentIpAddress();

    this->registry = new QRemoteObjectRegistryHost(QUrl(QString("tcp://%1:%2")
                               .arg(QString(ipAddress))
                               .arg(QString("8080/registry"))), this);


    if (!this->registry->waitForRegistry()) {
        ui->logWindow->append("Failed to run node: " + ipAddress + ":" + QString::number(8080));
        return;
    }

    ui->logWindow->append("Node running on address: " + ipAddress + ":" + QString::number(8080));
    this->counter = new CounterSimpleSource(this);
    this->registry->enableRemoting(this->counter);

    connect(this->registry->registry(), &QRemoteObjectRegistry::remoteObjectAdded, [this](QRemoteObjectSourceLocation entry)
    {
        ui->logWindow->append(QString("New node connected: %1").arg(entry.second.hostUrl.toString()));
        int counter = this->counter->counter();
        this->counter->setCounter(++counter);
    });
}

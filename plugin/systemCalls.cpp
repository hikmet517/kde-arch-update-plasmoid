#include "systemCalls.h"
#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>
#include <QMetaObject>
#include <QStringList>
#include <QTime>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3


systemCalls::systemCalls(QObject *parent) : QObject(parent)
{
    this->worker = new Worker;
    worker->moveToThread(&this->workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &systemCalls::checkUpdatesSignal, worker, &Worker::checkUpdates);
    connect(this, &systemCalls::upgradeSystemSignal, worker, &Worker::upgradeSystem);

    workerThread.start();
}



systemCalls::~systemCalls()
{
    workerThread.quit();
    workerThread.wait();
}


//https://karanbalkar.com/2014/02/detect-internet-connection-using-qt-5-framework/
Q_INVOKABLE bool systemCalls::isConnectedToNetwork()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);

        if (iface.flags().testFlag(QNetworkInterface::IsUp)
                && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            for (int j = 0; j < iface.addressEntries().count(); j++)
            {
                // got an interface which is up, and has an ip address
                if (result == false)
                    result = true;
            }
        }
    }

    return result;
}



Q_INVOKABLE void systemCalls::checkUpdates(bool namesOnly, QString checkupdatesCommand)
{
    if(worker->upgradeProcessRunning)
        return;

    worker->mutex = true;
    emit systemCalls::checkUpdatesSignal(namesOnly, checkupdatesCommand);

}


Q_INVOKABLE void systemCalls::upgradeSystem(bool konsoleFlag, bool yakuake, QString upgradeCommand)
{
    if(worker->upgradeProcessRunning)
        return;
    worker->mutex = true;
    worker->upgradeProcessRunning = true;
    emit systemCalls::upgradeSystemSignal(konsoleFlag, yakuake, upgradeCommand);
}


Q_INVOKABLE QStringList systemCalls::readCheckUpdates()
{
    QTime deliTime;
    if(worker->upgradeProcessRunning)
        return QStringList();
    deliTime = QTime::currentTime().addMSecs(500);
    while(worker->mutex == true)
        QCoreApplication::processEvents();

    worker->mutex = false;
    return worker->updates;
}

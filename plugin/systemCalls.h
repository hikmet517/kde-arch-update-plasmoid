#ifndef SYSTEMCALLS_H
#define SYSTEMCALLS_H
#include <QProcess>
#include <QStringList>
#include <QtConcurrent/QtConcurrentRun>
#include "worker.h"

/**
 * @brief The systemCalls class
 * Detailed: backend system calls for Arch Linux Update Notifier Plasmoid to run checkupdates/checkupdates-aur and upgrade system with various flags from settings
 * @author: Mike Harris
 */

class systemCalls : public QObject
{
    Q_OBJECT

    public:

    QThread workerThread;
    Worker *worker;

    /**
     * @brief systemCalls default contructor
     * @param parent
     */
    explicit systemCalls(QObject *parent = 0);

    ~systemCalls();

    Q_INVOKABLE void checkUpdates(bool namesOnly, QString checkupdatesCommand);
    Q_INVOKABLE void upgradeSystem(bool konsoleFlag, bool yakuake, QString upgradeCommand);

    void noop();

    /**
       @brief returns worker->updates
    */
    Q_INVOKABLE  QStringList readCheckUpdates();


    Q_INVOKABLE bool isConnectedToNetwork();

signals:
    /**
       @brief starts checkupdates on worker thread eventually returns worker->updates
       @details emits signal for worker thread to run checkupdates which saves checkupdates results in worker->updates. sets worker->updates to error string if no internet connection
       @param arguments- bool:namesOnly to strip version numbers, QString: command for checking updates
    */
    void checkUpdatesSignal(bool namesOnly, QString checkUpdatesCommand);
    /**
       @brief starts upgradeSystem on worker thread

       @details upgrades system. if konsoleflag=true show updates in console. sets worker->updates to empty list if success or an error string
    */
    void upgradeSystemSignal(bool konsoleFlag, bool yakuake, QString upgradeCommand);
};

#endif // SYSTEMCALLS_H

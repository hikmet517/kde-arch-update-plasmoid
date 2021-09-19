#ifndef SYSTEMCALLS_H
#define SYSTEMCALLS_H
#include <QStringList>
#include <QtConcurrent/QtConcurrentRun>


/**
 * @brief The systemCalls class
 * Detailed: backend system calls for Arch Linux Update Notifier Plasmoid to run checkupdates/checkupdates-aur and upgrade system with various flags from settings
 * @author: Mike Harris
 */

class systemCalls : public QObject
{
    Q_OBJECT

public:

     /**
     * @brief systemCalls default contructor
     * @param parent
     */
    explicit systemCalls(QObject *parent = 0);


    Q_INVOKABLE QStringList checkUpdates(bool namesOnly, QString checkupdatesCommand);
    Q_INVOKABLE void upgradeSystem(bool konsoleFlag, bool yakuakeFlag, QString upgradeCommand);
    Q_INVOKABLE bool isConnectedToNetwork();

signals:
    /**
       @brief Checks updates
       @details emits signal for worker thread to run checkupdates which saves checkupdates results in worker->updates. sets worker->updates to error string if no internet connection
       @param arguments- bool:namesOnly to strip version numbers, QString: command for checking updates
    */
    void checkUpdatesSignal(bool namesOnly, QString checkUpdatesCommand);

    /**
       @brief starts upgradeSystem

       @details upgrades system. if konsoleflag=true show updates in console.
    */
    void upgradeSystemSignal(bool konsoleFlag, bool yakuake, QString upgradeCommand);


private:
    QString prepareYakuake();
    bool runInYakuake(QString command);
    bool runInKonsole(QString command);
};

#endif // SYSTEMCALLS_H

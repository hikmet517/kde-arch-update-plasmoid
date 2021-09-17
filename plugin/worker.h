#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include <QString>

class QProcess;
class Worker : public QObject
{
    Q_OBJECT
private:
    QString prepareYakuake();
    bool runInYakuake(QString command);
    bool runInKonsole(QString command);

public:
    static bool mutex;
    static bool upgradeProcessRunning;

    Worker();
    QStringList updates;
    static bool wait;
signals:
    void readCheckUpdatesSignal(QStringList &results);

public slots:
    bool checkUpdates(bool namesOnly, QString checkupdatesCommand);
    void upgradeSystem(bool konsoleFlag, bool yakuakeFlag, QString upgradeCommand);
};
#endif

#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include <QString>

class QProcess;
class Worker : public QObject
{
    Q_OBJECT
private:
    bool syncDatabase(bool aur);
    void toggleYakuake(QString);
    QString prepareYakuake();
    QProcess *yakuakeProcess=NULL;

public:
    static bool mutex;
    static bool upgradeProcessRunning;

    Worker();
    QString getAURHelper();
    QStringList getAURHelperCommands(QString AURHelper);
    QStringList updates;
    static bool wait;
signals:
    void readCheckUpdatesSignal(QStringList &results);

public slots:
    bool checkUpdates(bool namesOnly, bool aur);
    void upgradeSystem(bool konsoleFlag, bool aur, bool noconfirm, bool yakuakeFlag, bool orphan, bool snapRefreshFlag);
};
#endif

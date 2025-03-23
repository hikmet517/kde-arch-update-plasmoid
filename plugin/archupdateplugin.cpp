#include "archupdateplugin.h"

#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QLoggingCategory>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QtNetwork/QNetworkInterface>

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3

ArchUpdatePlugin::ArchUpdatePlugin(QObject *parent)
    : QObject(parent)
{
#ifndef NDEBUG
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
#endif

    connect(this, &ArchUpdatePlugin::checkUpdatesSignal, this, &ArchUpdatePlugin::checkUpdates);
    connect(this, &ArchUpdatePlugin::upgradeSystemSignal, this, &ArchUpdatePlugin::upgradeSystem);
}

// https://karanbalkar.com/2014/02/detect-internet-connection-using-qt-5-framework/
Q_INVOKABLE bool ArchUpdatePlugin::isConnectedToNetwork()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++) {
        QNetworkInterface iface = ifaces.at(i);

        if (iface.flags().testFlag(QNetworkInterface::IsUp) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            for (int j = 0; j < iface.addressEntries().count(); j++) {
                // got an interface which is up, and has an ip address
                if (result == false)
                    result = true;
            }
        }
    }

    return result;
}

Q_INVOKABLE QStringList ArchUpdatePlugin::checkUpdates(bool namesOnly, QString checkupdatesCommand)
{
    qDebug() << "ArchUpdatePlugin::checkUpdates()";

    QProcess proc;

    QStringList splitted = checkupdatesCommand.split(QChar::Space);
    proc.start(splitted[0], QStringList(splitted.begin() + 1, splitted.end()));

    if (proc.waitForStarted(-1)) {
        while (proc.state() == QProcess::Running) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

        QString output = QString::fromLocal8Bit(proc.readAllStandardOutput());
        output = output.trimmed();
        if (output.isEmpty())
            return QStringList();

        QStringList updateList = output.split(QChar::LineFeed);
        QStringList updates;
        if (namesOnly) {
            for (const QString &line : updateList) {
                int idx = line.indexOf(QChar::Space);
                if (idx == -1)
                    idx = line.size();
                updates.append(line.mid(0, idx));
            }
        } else {
            updates = updateList;
        }
        // qDebug() << "updates: " << updates;
        return updates;
    }

    return QStringList();
}

QString ArchUpdatePlugin::prepareYakuake()
{
    QString session;
    bool foundTab = false;

    // check if yakuake already has a session named "arch updater"
    QProcess terminalIdListProcess;
    terminalIdListProcess.start(u8"qdbus", {u8"org.kde.yakuake", u8"/yakuake/sessions", u8"org.kde.yakuake.terminalIdList"});
    terminalIdListProcess.waitForFinished(-1);
    QString terminalIds = QString::fromLocal8Bit(terminalIdListProcess.readAllStandardOutput().simplified());
    QStringList terminalList = terminalIds.split(u',');
    for (const QString &str : terminalList) {
        QStringList arguments;
        arguments << u8"org.kde.yakuake" << u8"/yakuake/tabs" << u8"org.kde.yakuake.tabTitle" << str;
        QProcess getTitleProcess;
        getTitleProcess.start(u8"qdbus", arguments);
        getTitleProcess.waitForFinished(-1);
        QString tabTitle = QString::fromLocal8Bit(getTitleProcess.readAllStandardOutput().simplified());
        if (tabTitle == u8"arch updater") {
            session = QString::number(str.toInt() + 1);
            foundTab = true;
            break;
        }
    }

    if (!foundTab) {
        // if the session does not exist, create it
        QProcess addSessionProc;
        addSessionProc.start(u8"qdbus", {u8"org.kde.yakuake", u8"/yakuake/sessions", u8"org.kde.yakuake.addSession"});
        addSessionProc.waitForFinished(-1);
        QString term = QString::fromLocal8Bit(addSessionProc.readAllStandardOutput());

        QProcess setTitleProc;
        setTitleProc.start(u8"qdbus", {u8"org.kde.yakuake", u8"/yakuake/tabs", u8"setTabTitle", term, u8"arch updater"});
        setTitleProc.waitForFinished(-1);
        session = QString::number(term.toInt() + 1);
    }

    // focus yakuake
    QString yakuakeSession = QString::number(session.toInt() - 1);
    QProcess raiseSession;
    raiseSession.start(u8"qdbus", {u8"org.kde.yakuake", u8"/yakuake/sessions", u8"raiseSession", yakuakeSession});
    raiseSession.waitForFinished(-1);
    QProcess toggleWindow;
    toggleWindow.start(u8"qdbus", {u8"org.kde.yakuake", u8"/yakuake/window", u8"toggleWindowState"});
    toggleWindow.waitForFinished(-1);

    return session;
}

bool ArchUpdatePlugin::runInYakuake(QString command)
{
    QString session = prepareYakuake();
    QStringList args = QStringList() << u8"org.kde.yakuake" << QString(u8"/Sessions/%1").arg(session) << u8"runCommand" << command;
    QProcess proc;
    return proc.startDetached(u8"qdbus", args);
}

bool ArchUpdatePlugin::runInKonsole(QString command)
{
    QStringList args = QStringList() << u8"--hold" << u8"-e" << command;
    QProcess proc;
    return proc.startDetached(u8"konsole", args);
}

Q_INVOKABLE void ArchUpdatePlugin::upgradeSystem(bool konsoleFlag, bool yakuakeFlag, QString upgradeCommand)
{
    Q_UNUSED(konsoleFlag);

    QStringList args;

    QString command = QString(u8"bash -c '%1'").arg(upgradeCommand + QString(u8"; echo Done."));

    if (yakuakeFlag) {
        runInYakuake(command);
    } else {
        runInKonsole(command);
    }
}

#include "moc_archupdateplugin.cpp"

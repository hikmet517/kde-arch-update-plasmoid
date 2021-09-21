#include "systemCalls.h"

#include <QProcess>
#include <QString>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include <QStringList>
#include <QTime>
#include <QDebug>
#include <QLoggingCategory>
#include <qcoreapplication.h>

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3


systemCalls::systemCalls(QObject *parent) : QObject(parent)
{
#ifndef NDEBUG
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
#endif

    connect(this, &systemCalls::checkUpdatesSignal, this, &systemCalls::checkUpdates);
    connect(this, &systemCalls::upgradeSystemSignal, this, &systemCalls::upgradeSystem);
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


Q_INVOKABLE QStringList systemCalls::checkUpdates(bool namesOnly, QString checkupdatesCommand)
{
    qDebug() << "systemCalls::checkUpdates()";

    QProcess proc;

    QStringList splitted = checkupdatesCommand.split(' ');
    proc.start(splitted[0], QStringList(splitted.begin()+1, splitted.end()));

    if( proc.waitForStarted(-1) ) {
        while (proc.state() == QProcess::Running) {
            QCoreApplication::processEvents();
        }

        QString output = proc.readAllStandardOutput();
        output = output.trimmed();
        if ( output.isEmpty() )
            return QStringList();

        QStringList updateList = output.split('\n');
        QStringList updates;
        if( namesOnly ) {
            for (const QString &line : updateList) {
                int idx = line.indexOf(' ');
                if(idx == -1)
                    idx = line.size();
                updates.append(line.mid(0, idx));
            }
        }
        else {
            updates = updateList;
        }
        // qDebug() << "updates: " << updates;
        return updates;
    }

    return QStringList();
}


QString systemCalls::prepareYakuake()
{
    QString session;
    bool foundTab = false;

    // check if yakuake already has a session named "arch updater"
    QProcess terminalIdListProcess;
    terminalIdListProcess.start("qdbus", {"org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake.terminalIdList"});
    terminalIdListProcess.waitForFinished(-1);
    QString terminalIds(terminalIdListProcess.readAllStandardOutput().simplified());
    QStringList terminalList = terminalIds.split(',');
    foreach (const QString& str, terminalList) {
        QStringList arguments;
        arguments << "org.kde.yakuake" << "/yakuake/tabs" << "org.kde.yakuake.tabTitle" << str;
        QProcess getTitleProcess;
        getTitleProcess.start("qdbus", arguments);
        getTitleProcess.waitForFinished(-1);
        QString tabTitle(getTitleProcess.readAllStandardOutput().simplified());
        if(tabTitle == "arch updater") {
            session = QString::number(str.toInt() + 1);
            foundTab = true;
            break;
        }
    }

    if( !foundTab ) {
        // if the session does not exist, create it
        QProcess addSessionProc;
        addSessionProc.start("qdbus", {"org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake.addSession"});
        addSessionProc.waitForFinished(-1);
        QString term = addSessionProc.readAllStandardOutput();

        QProcess setTitleProc;
        setTitleProc.start("qdbus", {"org.kde.yakuake", "/yakuake/tabs", "setTabTitle", term, "arch updater"});
        setTitleProc.waitForFinished(-1);
        session = QString::number(term.toInt() + 1);
    }

    // focus yakuake
    QString yakuakeSession = QString::number(session.toInt() - 1);
    QProcess raiseSession;
    raiseSession.start("qdbus", {"org.kde.yakuake", "/yakuake/sessions", "raiseSession", yakuakeSession});
    raiseSession.waitForFinished(-1);
    QProcess toggleWindow;
    toggleWindow.start("qdbus", {"org.kde.yakuake", "/yakuake/window", "toggleWindowState"});
    toggleWindow.waitForFinished(-1);

    return session;
}


bool systemCalls::runInYakuake(QString command)
{
    QString session = prepareYakuake();
    QStringList args = QStringList() << "org.kde.yakuake" << QString("/Sessions/%1").arg(session)
                                     << "runCommand" << command;
    QProcess proc;
    return proc.startDetached("qdbus", args);
}


bool systemCalls::runInKonsole(QString command)
{
    QStringList args = QStringList() << "--hold" << "-e" << command;
    QProcess proc;
    return proc.startDetached("konsole", args);
}



Q_INVOKABLE void systemCalls::upgradeSystem(bool konsoleFlag, bool yakuakeFlag, QString upgradeCommand)
{
    Q_UNUSED(konsoleFlag);

    QStringList args;

    QString command = QString("bash -c '%1'").arg(upgradeCommand + "; echo Done.");

    if(yakuakeFlag) {
        runInYakuake(command);
    }
    else {
        runInKonsole(command);
    }
}

#include "worker.h"
#include "systemCalls.h"

#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include <QStandardPaths>
#include <QThread>
#include <QLoggingCategory>
#include <QProcess>
#include <QRegExp>

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3
#define NO_SPACE_LEFT 4

bool Worker::mutex;
bool Worker::upgradeProcessRunning;


Worker::Worker()
{
#ifndef NDEBUG
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
#endif
}


bool Worker::checkUpdates(bool namesOnly, QString checkupdatesCommand)
{
    qDebug() << "Worker::checkUpdates()";

    QProcess proc;

    QStringList splitted = checkupdatesCommand.split(' ');
    proc.start(splitted[0], QStringList(splitted.begin()+1, splitted.end()));

    if( proc.waitForStarted(-1) ) {
        if( proc.waitForFinished(-1) ) {
            QString output = proc.readAllStandardOutput();
            QStringList updateList = output.trimmed().split('\n');
            this->updates.clear();
            if( namesOnly ) {
                for (const QString &line : updateList) {
                    int idx = line.indexOf(' ');
                    if(idx == -1)
                        idx = line.size();
                    this->updates.append(line.mid(0, idx));
                }
            }
            else {
                this->updates = updateList;
            }
            this->mutex = false;
            // qDebug() << "updates: " << this->updates;
            return true;
        }
    }

    this->mutex = false;
    return false;
}


QString Worker::prepareYakuake()
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


bool Worker::runInYakuake(QString command)
{
    QString session = prepareYakuake();
    QStringList args = QStringList() << "org.kde.yakuake" << QString("/Sessions/%1").arg(session)
                                     << "runCommand" << command;
    QProcess proc;
    proc.start("qdbus", args);
    if( proc.waitForStarted(-1) )
        if( proc.waitForFinished(-1) )
            return true;
    return false;
}


bool Worker::runInKonsole(QString command)
{
    QStringList args = QStringList() << "--hold" << "-e" << command;
    QProcess proc;
    proc.start("konsole", args);
    if( proc.waitForStarted(-1) )
        if( proc.waitForFinished(-1) )
            return true;
    return false;
}


void Worker::upgradeSystem(bool konsoleFlag, bool yakuakeFlag, QString upgradeCommand)
{
    Q_UNUSED(konsoleFlag);

    QStringList args;

    QString command = QString("bash -c '%1'").arg(upgradeCommand);

    if(yakuakeFlag) {
        runInYakuake(command);
    }
    else {
        runInKonsole(command);
    }

    this->mutex = false;
    this->upgradeProcessRunning = false;
}

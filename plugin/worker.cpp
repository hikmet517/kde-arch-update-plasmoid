#include "systemCalls.h"
#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QLoggingCategory>

#include <algorithm>
#include <QProcess>
#include <QRegExp>

#include "worker.h"

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3
#define NO_SPACE_LEFT 4

bool Worker::mutex;
bool Worker::upgradeProcessRunning;

using std::sort;

Worker::Worker()
{
#ifndef NDEBUG
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
#endif
}

QString Worker::getAURHelper()
{
    QStringList aurHelperFilters;
    aurHelperFilters << "apacman" << "aura" << "aurget" << "bauerbill" << "pacaur" << "pacget"
                     << "pkgbuilder" << "trizen" << "yay" << "pikaur" << "pakku";

    QStringList aurHelperList;
    for(int i=0; i<aurHelperFilters.length(); i++) {
        QString path = QStandardPaths::findExecutable(aurHelperFilters[i]);
        if( !path.isEmpty() )
            aurHelperList.append(aurHelperFilters[i]);
    }

    sort(aurHelperList.begin(), aurHelperList.end());
    qDebug() << "AUR HELPER LIST";
    qDebug() << aurHelperList;

    if (aurHelperList.length() == 0)
        return nullptr;
    else
        return aurHelperList[0];
}


QStringList Worker::getAURHelperCommands(QString AURHelper)
{
    if (AURHelper == "apacman")
        return QStringList() << "apacman" << "-Syu" << "--noconfirm";

    else if (AURHelper == "aura")
        return QStringList() << "sudo" << "aura" << "-Ayu" << "--noconfirm" << "; " << "sudo" << "pacman" << "-Syu" << "--noconfirm";

    //aurget only upgrades aur need to run pacman too
    else if (AURHelper == "aurget")
        return QStringList() << "aurget" << "-Syu" << "--noconfirm" << " ; " << "sudo" << "pacman" << "-Syu" << "--noconfirm";

    else if (AURHelper == "bauerbill")
        return QStringList() << "bb-wrapper" << "-Syu" << "--aur" << "--noconfirm";

    else if (AURHelper == "pacaur")
        return QStringList() << "pacaur" << "-Syu" << "--noconfirm" << "--noedit";

    else if (AURHelper == "pacget")
        return QStringList() << "pacget" << "-Syu" << "--noconfirm";

    else if (AURHelper == "pkgbuilder")
        return QStringList() << "pkgbuilder" << "-Syu" << "--noconfirm";

    else if (AURHelper == "trizen")
        return QStringList() << "trizen" << "-Syu" << "--noconfirm";

    else if (AURHelper == "yay")
        return QStringList() << "yay" << "-Syu" << "--noconfirm" << "--sudoloop";

    else if (AURHelper == "pikaur")
        return QStringList() << "pikaur" << "-Syu" << "--noconfirm" << "--noedit";

    else if (AURHelper == "pakku")
        return QStringList() << "pakku" << "-Syu" << "--noconfirm";

    else
        return QStringList();
}

bool Worker::checkUpdates(bool namesOnly, bool aur)
{
    qDebug() << "Worker::checkUpdates";

    bool synced = syncDatabase(aur);
    if( synced ) {
        QProcess proc;
        if( aur ) {
            proc.start("yay", {"-Qu"});
        }
        else {
            proc.start("pacman", {"-Qu"});
        }

        if( proc.waitForStarted(-1) ) {
            if( proc.waitForFinished(-1) ) {
                QString output = proc.readAllStandardOutput();
                QStringList updateList = output.trimmed().split("\n");
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
                qDebug() << "updates: " << this->updates;
                return true;
            }
        }
    }
    this->mutex = false;
    return false;
}

bool Worker::syncDatabase(bool aur)
{
    qDebug() << "Worker::syncDatabase";
    QProcess proc;
    if( aur ) {
        proc.start("yay", {"-Syy"});
        if( proc.waitForStarted(-1) )
            if( proc.waitForFinished(-1) )
                return true;
    }
    else {
        proc.start("pacman", {"-Syy"});
        if( proc.waitForStarted(-1) )
            if( proc.waitForFinished(-1) )
                return true;
    }
    qDebug() << "Error in process:" << proc.program() << "args:" << proc.arguments();
    return false;
}

void Worker::toggleYakuake(QString session)
{
    QString yakuakeSession = QString::number(session.toInt() - 1);
    QProcess raiseSession;
    raiseSession.start("qdbus-qt5", {"org.kde.yakuake", "/yakuake/sessions", "raiseSession", yakuakeSession});
    raiseSession.waitForFinished(-1);
    QProcess toggleWindow;
    toggleWindow.start("qdbus-qt5", {"org.kde.yakuake", "/yakuake/window", "toggleWindowState"});
    toggleWindow.waitForFinished(-1);
}

QString Worker::prepareYakuake()
{
    // check if yakuake already has a session "arch updater")
    QProcess terminalIdListProcess;
    terminalIdListProcess.start("qdbus-qt5", {"org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake.terminalIdList"});
    terminalIdListProcess.waitForFinished(-1);
    QString terminalIds(terminalIdListProcess.readAllStandardOutput().simplified());
    QStringList terminalList = terminalIds.split(",");

    foreach (const QString& str, terminalList)
    {
        QStringList arguments;
        arguments << "org.kde.yakuake" << "/yakuake/tabs" << "org.kde.yakuake.tabTitle" << str;
        QProcess getTitleProcess;
        getTitleProcess.start("qdbus-qt5", arguments);
        getTitleProcess.waitForFinished(-1);
        QString tabTitle(getTitleProcess.readAllStandardOutput().simplified());
        if(tabTitle == "arch updater")
        {
            return QString::number(str.toInt() + 1);
        }
    }

    // if the session does not exist, create it
    QProcess addSessionProcess;
    addSessionProcess.start("qdbus-qt5", {"org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake.addSession"});
    addSessionProcess.waitForFinished(-1);
    QString terminal = addSessionProcess.readAllStandardOutput();

    QProcess setTitleProcess;
    setTitleProcess.start("qdbus-qt5", {"org.kde.yakuake", "/yakuake/tabs", "setTabTitle", terminal, "arch updater"});
    setTitleProcess.waitForFinished(-1);
    return QString::number(terminal.toInt() + 1);
}


void Worker::upgradeSystem(bool konsoleFlag, bool aur, bool noconfirm, bool yakuakeFlag, bool orphan, bool snapRefreshFlag)
{
    /*
     * Since QProcess causes a stack crash when stringing multiple commands we call a seperate shell script to call
     * the update process.
     *
     * Since the update process takes multiple commands seperated by ; we need to have the commands separated by , to
     * then create an IFS=',' in bash to get each command in an array.
     *
     *
     * The first argument is either konsole, yakuake or pkexec
     *
     * Ex :
     *      sudo pacman -Syu --noconfirm, echo update finished
     *
     *      in bash the array becomes
     *      ("sudo pacman -Syu --noconfirm;", "echo update finished;")
     *
     * Then all the commands are concatintated back into a large string and called in the bash script
     *
     */


    QProcess systemUpdateProcess;

    if (yakuakeFlag)
    {
        this->yakuakeProcess = new QProcess();
        this->yakuakeProcess->start("yakuake", QStringList());
        this->yakuakeProcess->waitForStarted(-1);
        prepareYakuake();
    }


    if (aur)
    {
        QString AURHelper = getAURHelper();
        if(AURHelper == nullptr)
        {
            qDebug () << "org.kde.archUpdate: you have no AUR helper installed.  Please install an AUR helper or disable AUR support";
            return;
        }
        // add to arguments aur helper specific command to update
        // apacman is -Syu versus yaort is -Syua etc
        qDebug() << "AUR HELPER: " << AURHelper;
        QStringList AURCommands = getAURHelperCommands(AURHelper);
        QStringList arguments;


        //remove --noconfirm if flag in settings not set
        if (noconfirm == false)
        {
            qDebug() << "remove --noconfirm";
            for(int i = 0; i < AURCommands.size(); i++)
            {
                if(AURCommands[i] == "--noconfirm" || AURCommands[i] == "--no-confirm") {
                    AURCommands.removeAt(i);
                    i--;
                }

                if (AURCommands[i] == "--noedit") {
                    AURCommands.removeAt(i);
                    i--;
                }
            }
            qDebug() << "done";
        }

        if (yakuakeFlag)
        {
            QString terminal = prepareYakuake();
            //          arguments << "yakuake" << "org.kde.yakuake" << "/Sessions/" + terminal << "runCommand" ;
            arguments << "yakuake" << terminal;

            toggleYakuake(terminal);
        }

        else   // use Konsole
        {
            //run /bin/bash -c konsole --hold -e 'sh -c " *aur helper commnads* ; echo Update Finished "

            // start with konsole  --hold -e  **aur helper**
            arguments << "konsole";

        }
        for (int i = 0; i < AURCommands.size() - 1; i++)
            arguments << AURCommands[i];

        arguments << AURCommands[AURCommands.size() - 1] + ",";


        if(orphan)
            arguments << "echo," << "echo" << "Cleaning" << " Orphans," << "sudo" << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm,";

        if(snapRefreshFlag)
            arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

        arguments << "echo," << "echo" << "----------------," <<  "echo" << "Update" << "Finished";
        //start system update process
        qDebug() << "AUR ARGS " << arguments;
        systemUpdateProcess.start("/usr/bin/ArchUpdater", arguments);
    }

    else //no aur flag
    {
        // if user selects show in konsole in settings display in konsole
        if (konsoleFlag)
        {
            QProcess proc;
            proc.start("konsole", {"--hold", "-e", "bash -c 'sudo pacman -Syu'"});
            proc.waitForFinished(-1);

            // QStringList arguments;
            // // /bin/bash -c konsole --hold -e 'sh -c "sudo pacman -Syu ; echo Update Finished'""
            // arguments << "konsole" << "sudo" << "pacman" << "-Syu,";

            // if(orphan)
            //     arguments << "echo," << "echo" << "Cleaning" << " Orphans," << "sudo" << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm,";

            // if(snapRefreshFlag)
            //     arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

            // arguments << "echo," << "echo" << "----------------," <<  "echo" << "Update" << "Finished";
            // systemUpdateProcess.start("/usr/bin/ArchUpdater", arguments);
        }

        // if user selects show in yakuake in settings display in yakuake
        if (yakuakeFlag)
        {
            QString terminal = prepareYakuake();
            QProcess proc;
            proc.start("qdbus", {"org.kde.yakuake", "/Sessions/" + terminal, "runCommand", "bash -c 'sudo pacman -Syu'"});
            toggleYakuake(terminal);
            proc.waitForFinished(-1);

            // QStringList arguments;
            // QString terminal = prepareYakuake();
            // arguments << "yakuake" << terminal << "sudo" << "pacman" << "-Syu,";

            // if(orphan)
            //     arguments << "echo," << "echo" << "Cleaning" << " Orphans," << "sudo" << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm,";

            // if(snapRefreshFlag)
            //     arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

            // arguments << "echo," << "echo" << "----------------," <<  "echo" << "Update" << "Finished";
            // systemUpdateProcess.start("/usr/bin/ArchUpdater", arguments);
            // qDebug() << "ARGS " << arguments;
            // toggleYakuake(terminal);
        }

        //run in background
        if (yakuakeFlag == false && konsoleFlag == false)
        {
            //pexec pacman -Syu --noconfirm
            QStringList arguments;
            arguments << "pkexec" << "/usr/bin/pacman" << "-Syu" << "--noconfirm,";

            if(orphan)
                arguments << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm";

            if(snapRefreshFlag)
                arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

            systemUpdateProcess.start("usr/bin/ArchUpdater", arguments);
        }
    }
    // if (systemUpdateProcess.waitForStarted(-1))
    // {
    //     bool res = systemUpdateProcess.waitForFinished(-1);
    //     if( res )
    //     {
    //         qDebug() << "org.kde.archUpdate: Upgrade process finished";
    //         if( systemUpdateProcess.exitCode() == 0 )
    //             qDebug() << "org.kde.archUpdate: Upgrade has succeded";
    //         else
    //             qDebug() << "org.kde.archUpdate: Upgrade has failed";
    //     }
    //     else
    //     {
    //         qDebug() << "org.kde.archUpdate: Cannot finish update";
    //     }
    // }
    // else
    // {
    //     qDebug() << "org.kde.archUpdate: Cannot start system upgrade process";
    // }

    this->upgradeProcessRunning = false;
    this->mutex = false;
    delete this->yakuakeProcess;
}

import QtQuick
import QtQuick.Layouts

import org.kde.plasma.plasmoid
import org.kde.plasma.core as PlasmaCore

import com.github.archupdate


PlasmoidItem {
    SystemCalls {
        id: backend
    }
    id: main

    // for debugging
    // Plasmoid.preferredRepresentation: Plasmoid.compactRepresentation

    property string appName: "Arch Updater"
    property string version: "1.0"
    property int iconSize: PlasmaCore.Units.iconSizes.smallMedium
    property int leftColumnWidth: iconSize + Math.round(PlasmaCore.Units.gridUnit / 2)
    property string appletIcon: "chosen"
    property var updatesPending: 0
    property var theModel: updateListModel
    property var namesOnly: plasmoid.configuration.hideVersion
    property bool internetCheck: false

    onNamesOnlyChanged: timer.restart()

    Plasmoid.icon: plasmoid.file("images", appletIcon)
    Plasmoid.compactRepresentation: CompactRepresentation {}
    Plasmoid.fullRepresentation: FullRepresentation {}
    Plasmoid.status: {
        if(updatesPending > 0 || updatesPending==="?") {
            return PlasmaCore.Types.ActiveStatus;
        }
        return PlasmaCore.Types.PassiveStatus;
    }

    PlasmaCore.DataSource {
        id: notificationSource
        engine: "notifications"
        connectedSources: "org.freedesktop.Notifications"
    }
    ListModel {
        id: updateListModel
    }

    //used to initialize start up
    Timer {
        id: timer
        interval: 1000
        running: true
        repeat: false
        onTriggered: refresh()
    }


    Timer {
        id: noInternetRecheckTimer
        interval: 60000
        repeat: false
        onTriggered: {
            console.log("com.github.archupdate: rechecking internet");
            refresh()
        }
    }

    function refresh() {
        console.log("com.github.archupdate: checking internet")
        if(!backend.isConnectedToNetwork() && internetCheck==false){
            noInternetRecheckTimer.start();
            console.log("com.github.archupdate: Timer started");
            updateListModel.clear();
            updatesPending="?";
            updateListModel.append({"text":"Not connected to internet. Rechecking internet connection in 1 minute"});
            internetCheck=true;
            return;
        }

        if(internetCheck && !backend.isConnectedToNetwork()){
            console.log("com.github.archupdate: still no internet connection");
            updateListModel.clear();
            updatesPending="?";
            updateListModel.append({"text":"No internet connection"});
            internetCheck=false;
            return;
        }

        updateListModel.clear();
        var packageList;
        console.log("NAMES ONLY " + plasmoid.configuration.hideVersion);


        packageList = backend.checkUpdates(plasmoid.configuration.hideVersion,
                             plasmoid.configuration.checkUpdatesCommand);
        //append packages to full representation list
        for (var i = 0; i < packageList.length; i++) {
            updateListModel.append({"text": packageList[i]});
        }
        //counter on CompactRepresentation
        updatesPending = packageList.length;
    }
}

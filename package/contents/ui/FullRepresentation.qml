import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 3.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.private.archUpdate 1.0


Item {
    id: fullRep
    property bool discard: false
    width: theme.implicitWidth
    height: theme.implicitHeight

    SystemCalls {
        id: backend
    }

    PlasmaExtras.ScrollArea {
        id: updatesScroll

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: buttonRow.top
        }

        ListView {
            id: updateListView
            anchors.fill: parent
            width: parent.width
            height: parent.height
            model: main.theModel
            delegate: Item {
                width: parent.width
                height: theme.mSize(theme.defaultFont).height + PlasmaCore.Units.largeSpacing
                Text {
                    text: modelData
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                    color: theme.textColor
                }
            }
            snapMode: ListView.SnapToItem
        }
    }

    RowLayout {
        id: buttonRow
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        PlasmaComponents.Button {
            id: updateButton
            text: "Update System"
            onClicked: function () {
                backend.upgradeSystem(plasmoid.configuration.konsoleFlag,
                                      plasmoid.configuration.yakuakeFlag,
                                      plasmoid.configuration.upgradeCommand);

                main.updatesPending=0;
                main.theModel.clear();
            }
        }
        PlasmaComponents.Button {
            id: checkUpdatesButton
            text: "Check for Updates"
            onClicked: function () {
                main.refresh()
            }
        }
    }
}

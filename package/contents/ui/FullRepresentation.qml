import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.plasma.plasmoid
import org.kde.plasma.components as PlasmaComponents
import org.kde.plasma.core as PlasmaCore
import org.kde.plasma.extras as PlasmaExtras

import com.github.archupdate


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

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.plasma.plasmoid
import org.kde.plasma.components as PlasmaComponents3
import org.kde.kirigami as Kirigami
import org.kde.plasma.extras as PlasmaExtras

import com.github.private.archupdate


PlasmaExtras.Representation {
    id: fullRep

    Layout.minimumWidth: Kirigami.Units.gridUnit * 18
    Layout.minimumHeight: Kirigami.Units.gridUnit * 18
    Layout.maximumWidth: Kirigami.Units.gridUnit * 80
    Layout.maximumHeight: Kirigami.Units.gridUnit * 40

    PlasmaComponents3.ScrollView {
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
            model: root.theModel
            delegate: Item {
                width: updateListView.width
                height: Kirigami.Theme.defaultFont.pixelSize * 2
                Text {
                    text: modelData
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                    color: Kirigami.Theme.textColor
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

        PlasmaComponents3.Button {
            id: updateButton
            text: "Update System"
            onClicked: function () {
                ArchUpdatePlugin.upgradeSystem(plasmoid.configuration.konsoleFlag,
                                               plasmoid.configuration.yakuakeFlag,
                                               plasmoid.configuration.upgradeCommand);

                root.updatesPending=0;
                root.theModel.clear();
            }
        }
        PlasmaComponents3.Button {
            id: checkUpdatesButton
            text: "Check for Updates"
            onClicked: function () {
                root.refresh()
            }
        }
    }
}

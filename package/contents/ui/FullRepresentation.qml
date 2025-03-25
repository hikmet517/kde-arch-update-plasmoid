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
                height: Kirigami.Theme.defaultFont.pixelSize + Kirigami.Units.gridUnit
                Text {
                    text: modelData
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                    color: Kirigami.Theme.textColor
                }
            }
            snapMode: ListView.SnapToItem

            Loader {
                anchors.centerIn: parent
                width: parent.width - (Kirigami.Units.gridUnit * 4)

                active: updateListView.count === 0
                visible: active
                asynchronous: true

                sourceComponent: noUpdate
            }

            Component {
                id: noUpdate

                PlasmaExtras.PlaceholderMessage {
                    anchors.centerIn: parent
                    width: parent.width

                    iconName: "checkmark"
                    text: i18n("No update available")
                }
            }
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

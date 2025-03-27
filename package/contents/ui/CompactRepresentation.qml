import QtQuick
import QtQuick.Layouts
/* import QtGraphicalEffects */

import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami
import org.kde.plasma.components as PlasmaComponents3
import org.kde.kquickcontrolsaddons as KQuickControlsAddons

import com.github.private.archupdate


Item {
    id: compactRep
    property int checkInterval: Plasmoid.configuration.checkInterval * 60000
    onCheckIntervalChanged: refreshTimer.interval=checkInterval

    Layout.maximumWidth: inPanel ? Kirigami.Units.iconSizeHints.panel : -1
    Layout.maximumHeight: inPanel ? Kirigami.Units.iconSizeHints.panel : -1


    Kirigami.Icon {
        id: compactIcon
        source: Qt.resolvedUrl("../images/arch-plasmoid.svg")
        anchors.fill: parent
        active: mouseArea.containsMouse
    }

    Timer {
        id: refreshTimer
        interval: checkInterval
        running: true
        repeat: true
        onTriggered: function () {
            root.refresh()
        }
    }

    Rectangle {
        id: circle
        height: label.height
        width: label.width + 4
        radius: width * 0.40
        /* color: PlasmaCore.ColorScope.backgroundColor */
        opacity: 0.8
        visible: root.updatesPending > 0 || updatesPending === "?"
        anchors {
            right: parent.right
            top: parent.top
        }

        PlasmaComponents3.Label {
            id: label
            text: root.updatesPending > 99 || root.updatesPending < 0 ? "99+" : root.updatesPending
            font.pixelSize: Kirigami.Theme.smallFont.pixelSize
            font.bold: true
            anchors.centerIn: parent
            visible: circle.visible
        }

        layer.enabled: true
        /* layer.effect: DropShadow { */
        /*     horizontalOffset: 0 */
        /*     verticalOffset: 0 */
        /*     radius: 1 */
        /*     samples: 1 + radius * 2 */
        /*     color: Qt.rgba(0, 0, 0, 0.3) */
        /* } */
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.expanded = !root.expanded
        hoverEnabled: true
    }
}

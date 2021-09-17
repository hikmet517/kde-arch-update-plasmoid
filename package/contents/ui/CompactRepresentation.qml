import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons
import org.kde.private.archUpdate 1.0;


Item {
    id: compactRep
    property int checkInterval: plasmoid.configuration.checkInterval * 60000
    onCheckIntervalChanged: refreshTimer.interval=checkInterval

    Layout.maximumWidth: inPanel ? PlasmaCore.Units.iconSizeHints.panel : -1
    Layout.maximumHeight: inPanel ? PlasmaCore.Units.iconSizeHints.panel : -1


    PlasmaCore.IconItem {
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
            main.refresh()
        }
    }
    Rectangle {
        id: circle
        height: label.height
        width: label.width + 4 * PlasmaCore.Units.devicePixelRatio
        radius: width * 0.40
        color: PlasmaCore.ColorScope.backgroundColor
        opacity: 0.8
        visible: main.updatesPending > 0 || updatesPending === "?"
        anchors {
            right: parent.right
            top: parent.top
        }

        PlasmaComponents.Label {
            id: label
            text: main.updatesPending > 99 || main.updatesPending < 0 ? "99+" : main.updatesPending
            font.pixelSize: PlasmaCore.Theme.smallestFont.pixelSize
            font.bold: true
            anchors.centerIn: parent
            visible: circle.visible
        }

        // layer.enabled: true
        // layer.effect: DropShadow {
        //     horizontalOffset: 0
        //     verticalOffset: 0
        //     radius: PlasmaCore.Units.devicePixelRatio
        //     samples: PlasmaCore.Units.devicePixelRatio
        //     color: Qt.rgba(0, 0, 0, 0.5)
        // }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: plasmoid.expanded = !plasmoid.expanded
        hoverEnabled: true
    }
}

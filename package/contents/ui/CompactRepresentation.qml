import QtQuick
import QtQuick.Layouts
import QtGraphicalEffects

import org.kde.plasma.plasmoid
import org.kde.plasma.core as PlasmaCore
import org.kde.plasma.components as PlasmaComponents
import org.kde.kquickcontrolsaddons as KQuickControlsAddons

import com.github.archupdate


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

        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 0
            verticalOffset: 0
            radius: 1
            samples: 1 + radius * 2
            color: Qt.rgba(0, 0, 0, 0.3)
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: plasmoid.expanded = !plasmoid.expanded
        hoverEnabled: true
    }
}

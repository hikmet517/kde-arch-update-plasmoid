import QtQuick 2.2
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons
import org.kde.private.archUpdate 1.0;


Item {
    id: compactRep
    property int checkInterval: plasmoid.configuration.checkInterval * 60000
    onCheckIntervalChanged: function(){refreshTimer.interval=checkInterval}
    
    Layout.minimumWidth: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? height : PlasmaCore.Units.iconSizes.small
    Layout.minimumHeight: plasmoid.formFactor === PlasmaCore.Types.Vertical ? width
    : (PlasmaCore.Units.iconSizes.small + 2 * PlasmaCore.Theme.mSize(PlasmaCore.Theme.defaultFont).height)

    Layout.maximumWidth: inPanel ? PlasmaCore.Units.iconSizeHints.panel : -1
    Layout.maximumHeight: inPanel ? PlasmaCore.Units.iconSizeHints.panel : -1
    
    
    
    PlasmaCore.IconItem {
        id: compactIcon
        source: Qt.resolvedUrl("../images/arch-plasmoid.svg")
        anchors.fill: parent
    }
    Timer {
        id: refreshTimer
        interval: checkInterval
        running: true
        repeat: true
        onTriggered: main.refresh()
    }
    Rectangle {
        id: circle
        width: Math.max(height, label.width - PlasmaCore.Units.devicePixelRatio * 2)
        height: PlasmaCore.Units.gridUnit - PlasmaCore.Units.devicePixelRatio * 2
        radius: width * 0.32
        color: PlasmaCore.ColorScope.backgroundColor
        opacity: 0.8
        visible: main.updatesPending > 0 || updatesPending==="?"
        anchors {
            right: parent.right
            top: parent.top
        }
    }

    PlasmaComponents.Label {
        id: label
        text: main.updatesPending > 99 || main.updatesPending<0 ? "99+" : main.updatesPending
        font.pixelSize: Math.max(compactIcon.height/4, PlasmaCore.Theme.smallestFont.pixelSize * 0.8)
        anchors.centerIn: circle
        visible: circle.visible
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: plasmoid.expanded = !plasmoid.expanded
        hoverEnabled: true
    }
}

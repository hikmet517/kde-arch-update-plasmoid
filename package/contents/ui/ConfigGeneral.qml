import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.private.archUpdate 1.0

Item {
    id: item2
    SystemCalls {
        id: backend
    }
    property alias cfg_checkInterval: checkInterval.value
    property alias cfg_hideVersion: hideVersionCheckBox.checked
    property alias cfg_konsoleFlag: konsoleCheckBox.checked
    property alias cfg_aurSupportFlag: aurSupportCheckBox.checked
    property alias cfg_noConfirmAURFlag: noConfirmAURCheckBox.checked
    property alias cfg_yakuakeFlag: yakuakeCheckbox.checked
    property alias cfg_orphanFlag: cleanOrphanCheckbox.checked
    property alias cfg_snapRefreshFlag: snapRefreshCheckbox.checked

    GridLayout {
        id: checkboxGroup
        height: 390
        anchors.left: parent.left
        anchors.right: parent.right
        Layout.fillWidth: true
        rowSpacing: 10
        columnSpacing: 10
        columns: 3
        Label {
            text: i18n("Check Every (Minutes)")
        }

        SpinBox {
            id: checkInterval
            Layout.columnSpan: 2
            decimals: 0
            value: 15
            minimumValue: 1
            maximumValue: 999
            onValueChanged: cfg_checkInterval = value
        }
        Label {
            text: i18n("Hide the updates version number:")
        }
        CheckBox {
            id: hideVersionCheckBox
            Layout.columnSpan: 2
        }

        Label {
            text: i18n("Show upgrade on konsole:")
        }
        CheckBox {
            id: konsoleCheckBox
            Layout.columnSpan: 2
            onClicked: function () {
                if (konsoleCheckBox.checked) {
                    yakuakeCheckbox.checked = false
                }
            }
        }

        Label {
            text: i18n("Show upgrade process in yakuake:")
        }

        CheckBox {
            id: yakuakeCheckbox
            Layout.columnSpan: 2
            onClicked: function () {
                if (yakuakeCheckbox.checked) {
                    konsoleCheckBox.checked = false
                }
            }
        }
        Label {
            id: aurText
            text: i18n("AUR support:")
        }
        CheckBox {
            id: aurSupportCheckBox
        }

        Label {
            text: i18n("Enabled") + "  (" + i18n(
                      "Requires ") + "checkupdates-aur)"
            Layout.columnSpan: 1
        }

        Item {
            id: spacer1
            Layout.fillWidth: true
        }

        CheckBox {
            id: noConfirmAURCheckBox
        }

        Label {
            id: noConfirmAURText
            text: "--noconfirm aur"
        }

        Label {
            text: i18n("Clean orphan packages after upgrade:")
        }

        CheckBox {
            id: cleanOrphanCheckbox
            Layout.columnSpan: 2
        }

        Label{
            text: i18n("Refresh snap packages during upgrade process:")
        }

        CheckBox {
            id: snapRefreshCheckbox
            Layout.columnSpan: 2
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/

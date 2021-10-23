import QtQuick 2.5
import QtQuick.Controls 2.5 as QtControls
import QtQuick.Layouts 1.1 as QtLayouts
import org.kde.kirigami 2.5 as Kirigami
import org.kde.plasma.core 2.0 as PlasmaCore


Kirigami.FormLayout {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right

    property alias cfg_checkInterval: checkInterval.value
    property alias cfg_hideVersion: hideVersionCheckBox.checked
    property alias cfg_konsoleFlag: konsoleRadio.checked
    property alias cfg_yakuakeFlag: yakuakeRadio.checked
    property alias cfg_checkUpdatesCommand: checkCommandField.text
    property alias cfg_upgradeCommand: upgradeCommandField.text

    QtLayouts.RowLayout {
        Kirigami.FormData.label: i18nc("@label:spinbox", "Check for updates every:")

        QtControls.SpinBox {
            id: checkInterval
            from: 1
            to: 999
            value: 15
            stepSize: 1
            onValueChanged: cfg_checkInterval = value
        }

        QtControls.Label {
            text: i18ncp("@item:valuesuffix spacing to number + unit (minutes)", "minute", "minutes")
        }
    }

    QtControls.CheckBox {
        id: hideVersionCheckBox
        text: i18nc("@option:check", "Hide version numbers of updates")
    }

    Item {
        Kirigami.FormData.isSection: true
    }


    QtControls.RadioButton {
        id: konsoleRadio
        Kirigami.FormData.label: i18n("Show upgrade process in:")
        text: i18nc("@option:radio", "Konsole")
    }

    QtControls.RadioButton {
        id: yakuakeRadio
        text: i18nc("@option:radio", "Yakuake")
    }

    Item {
        Kirigami.FormData.isSection: true
    }

    QtControls.TextField {
        id: checkCommandField
        Kirigami.FormData.label: i18n("Command for checking updates:")
        QtLayouts.Layout.fillWidth: true

        QtControls.ToolTip {
            text: i18n("Will be run in background, should not include sudo.\nOutput format should be similar to the output of checkupdates. (package oldVersion -> newVersion)")
        }
    }

    QtControls.TextField {
        id: upgradeCommandField
        Kirigami.FormData.label: i18n("Command for upgrading system:")
        QtLayouts.Layout.fillWidth: true

        QtControls.ToolTip {
            text: i18n("Will be run in Konsole or Yakuake")
        }
    }

    Item {
        Kirigami.FormData.isSection: true
    }
}

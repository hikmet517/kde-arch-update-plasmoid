import QtQuick 2.5
import QtQuick.Controls 2.5 as QtControls
import QtQuick.Layouts 1.1 as QtLayouts
import org.kde.kirigami 2.5 as Kirigami


Kirigami.FormLayout {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right

    property alias cfg_checkInterval: checkInterval.value
    property alias cfg_hideVersion: hideVersionCheckBox.checked
    property alias cfg_konsoleFlag: konsoleRadio.checked
    property alias cfg_yakuakeFlag: yakuakeRadio.checked
    property alias cfg_backgroundFlag: backgroundRadio.checked
    property alias cfg_aurSupportFlag: aurSupportCheckBox.checked
    property alias cfg_noConfirmAURFlag: noConfirmAURCheckBox.checked
    property alias cfg_orphanFlag: cleanOrphanCheckbox.checked
    property alias cfg_snapRefreshFlag: snapRefreshCheckbox.checked

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

    QtControls.RadioButton {
        id: backgroundRadio
        text: i18nc("@option:radio", "Do not show (run it in background)")
    }


    Item {
        Kirigami.FormData.isSection: true
    }

    QtControls.CheckBox {
        Kirigami.FormData.label: i18n("AUR Support:")
        id: aurSupportCheckBox
        text: i18nc("@option:check", "Enable")
    }

    QtControls.CheckBox {
        id: noConfirmAURCheckBox
        text: i18nc("@option:check", "Use --noconfirm")
        enabled: aurSupportCheckBox.checked
    }

    QtControls.CheckBox {
        id: cleanOrphanCheckbox
        text: i18nc("@option:check", "Clean orphan packages after upgrade")
        enabled: aurSupportCheckBox.checked
    }

    QtControls.CheckBox {
        id: snapRefreshCheckbox
        text: i18nc("@option:check", "Refresh snap packages during upgrade process")
        enabled: aurSupportCheckBox.checked
    }
}

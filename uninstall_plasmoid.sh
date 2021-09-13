#!/bin/sh

sudo rm -rf /usr/lib/qt/qml/org/kde/private/archUpdate/
sudo rm /usr/bin/ArchUpdater
sudo rm -rf /usr/share/plasma/plasmoids/org.kde.archUpdate/
sudo rm /usr/share/metainfo/org.kde.archUpdate.appdata.xml

killall plasmashell && kstart5 plasmashell

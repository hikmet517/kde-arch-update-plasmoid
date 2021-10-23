#!/bin/sh

sudo rm -rf /usr/lib/qt/qml/org/kde/private/archUpdate/
sudo rm -rf /usr/share/plasma/plasmoids/org.kde.archUpdate/
sudo rm /usr/share/metainfo/org.kde.archUpdate.appdata.xml
sudo rm /usr/share/kservices5/plasma-applet-org.kde.archUpdate.desktop

killall plasmashell && kstart5 plasmashell

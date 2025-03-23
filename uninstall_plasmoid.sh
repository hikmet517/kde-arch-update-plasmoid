#!/bin/sh

xargs sudo rm < build/install_manifest.txt
sudo rm -rf /usr/share/plasma/plasmoids/com.github.archupdate/

# plasmashell --replace
# killall plasmashell && kstart plasmashell

# /usr/lib/qt6/plugins/plasma/applets/com.github.archupdate.so
# /usr/share/plasma/plasmoids/com.github.archupdate/metadata.desktop
# /usr/share/plasma/plasmoids/com.github.archupdate/contents/config/config.qml
# /usr/share/plasma/plasmoids/com.github.archupdate/contents/config/main.xml
# /usr/share/plasma/plasmoids/com.github.archupdate/contents/images/arch-plasmoid.svg
# /usr/share/plasma/plasmoids/com.github.archupdate/contents/ui/FullRepresentation.qml
# /usr/share/plasma/plasmoids/com.github.archupdate/contents/ui/CompactRepresentation.qml
# /usr/share/plasma/plasmoids/com.github.archupdate/contents/ui/main.qml
# /usr/share/plasma/plasmoids/com.github.archupdate/contents/ui/ConfigGeneral.qml
# /usr/share/plasma/plasmoids/com.github.archupdate/metadata.json
# /usr/share/metainfo/com.github.archupdate.appdata.xml

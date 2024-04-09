set(plasmoidplugin_SRCS
    plasmoidplugin.cpp
    systemCalls.cpp
)

kcoreaddons_add_plugin(com.github.archupdate
    SOURCES
    ${plasmoidplugin_SRCS}
    INSTALL_NAMESPACE
    "plasma/applets"
)

target_link_libraries(com.github.archupdate
    Qt::Gui
    Qt::Widgets
    Plasma::Plasma
    Plasma::Plasma5Support
)

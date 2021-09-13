set(plasmoidplugin_SRCS
    plasmoidplugin.cpp
    systemCalls.cpp
    worker.cpp
    )

add_library(plasmoidplugin SHARED ${plasmoidplugin_SRCS})

target_link_libraries(plasmoidplugin
    PRIVATE
        Qt::Core
        Qt::Qml
        Qt::Widgets
        KF5::CoreAddons)

install(TARGETS plasmoidplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/archUpdate)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/archUpdate)

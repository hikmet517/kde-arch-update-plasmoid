ecm_add_qml_module(archupdateplugin
  URI "com.github.private.archupdate"
  VERSION 1.0
  GENERATE_PLUGIN_SOURCE
  DEPENDENCIES QtCore
)

target_sources(archupdateplugin
  PRIVATE
  "archupdateplugin.cpp"
  "archupdateplugin.h"
)

target_link_libraries(archupdateplugin
  PRIVATE
  Qt::Core
  Qt::Gui
  Plasma::Plasma
)

ecm_finalize_qml_module(archupdateplugin)

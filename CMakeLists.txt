cmake_minimum_required(VERSION 3.16.0)

project(kde-arch-update-plasmoid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_DEP_VERSION "6.0.3")
set(QT_MIN_VERSION "6.6.0")
set(KF6_MIN_VERSION 5.240.0)
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(ECMGenerateExportHeader)
include(ECMInstallIcons)
include(KDEPackageAppTemplates)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(KDEClangFormat)
include(ECMDeprecationSettings)
include(ECMQmlModule)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED
    Core
    Gui
    DBus
    Network
    Quick
    Qml
    Widgets
    Test
    Core5Compat
)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    Config
    CoreAddons
    Declarative
    GlobalAccel
    Holidays
    I18n
    KIO
    KCMUtils
    Notifications
    Runner
    Service
    Sonnet
    UnitConversion
    XmlGui
    NewStuff
    JobWidgets
)

add_definitions(
    -DQT_DEPRECATED_WARNINGS
    -DQT_NO_URL_CAST_FROM_STRING
)

ecm_set_disabled_deprecation_versions(
    QT 6.5.0
)

find_package(Plasma5Support ${PROJECT_DEP_VERSION} REQUIRED)
find_package(Plasma ${PROJECT_DEP_VERSION} REQUIRED)
find_package(PlasmaQuick ${PROJECT_DEP_VERSION} REQUIRED)

add_definitions(
    -DQT_DEPRECATED_WARNINGS
    -DQT_NO_URL_CAST_FROM_STRING
)

add_subdirectory(plugin)

plasma_install_package(package com.github.archupdate)

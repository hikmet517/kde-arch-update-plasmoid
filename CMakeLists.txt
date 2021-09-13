cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(kde-arch-update-plasmoid)

set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.0.0")

add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Quick)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma)
add_subdirectory(plugin)

install(PROGRAMS ArchUpdater DESTINATION "bin")
plasma_install_package(package org.kde.archUpdate)

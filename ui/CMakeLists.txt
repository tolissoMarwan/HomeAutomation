# UI CMakeLists.txt (home-automation/ui/CMakeLists.txt)

cmake_minimum_required(VERSION 3.28)

project(Ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_add_executable(ui
    MANUAL_FINALIZATION
    "main.cpp"
    "widgets/MainWindow.h"
    "widgets/MainWindow.cpp"
    "widgets/MainWindow.ui"
    "qml/resources.qrc"
)

qt_add_resources(ui
    qml/resources.qrc
)

target_include_directories(ui PRIVATE
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_SOURCE_DIR}/data/include
    ${CMAKE_SOURCE_DIR}/logic/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ui
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        Qt6::Gui
        Qt6::Mqtt
        Qt6::Charts
    PUBLIC
        logic
)

qt_finalize_executable(ui)

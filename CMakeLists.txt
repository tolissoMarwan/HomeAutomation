cmake_minimum_required(VERSION 3.21)
project(homeautomation VERSION 1.0 LANGUAGES CXX)

# Qt configuration
set(QT_VERSION 6)

# Modern CMake policies
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
    Core
    Widgets
    Quick
    Mqtt
    Sql
)

qt_standard_project_setup()
#qt_add_executable(homeautomation)

# Subprojects (order matters)
add_subdirectory(common)
#add_subdirectory(data)
#add_subdirectory(logic)
#add_subdirectory(ui)

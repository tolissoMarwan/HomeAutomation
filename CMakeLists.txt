# Root CMakeLists.txt (home-automation/CMakeLists.txt)

cmake_minimum_required(VERSION 3.28)

project(HomeAutomation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS REQUIRED
    Core
    Mqtt
    Widgets
    Sql
    Gui
    Charts
    #Test
    REQUIRED
) # Add Sql, Test, and other modules as needed

qt_standard_project_setup()

# Subdirectories - Each has its own CMakeLists.txt
add_subdirectory(common)
add_subdirectory(data)
add_subdirectory(logic)
add_subdirectory(ui)
# add_subdirectory(tests) # Optional, if you want to build tests by default


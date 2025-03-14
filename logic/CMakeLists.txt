# LOGIC CMakeLists.txt (home-automation/logic/CMakeLists.txt)

cmake_minimum_required(VERSION 3.28)

project(Logic LANGUAGES CXX)

add_library(logic
    "src/DeviceManager.cpp"
    "src/SensorDataProcessor.cpp"
    "include/DeviceManager.h"
    "include/SensorDataProcessor.h"
)

target_include_directories(logic
    PUBLIC
        include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/common/include
        ${CMAKE_SOURCE_DIR}/data/include
)

target_link_libraries(logic
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        Qt6::Gui
        Qt6::Mqtt
        Qt6::Charts
    PUBLIC
        data
)

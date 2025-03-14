cmake_minimum_required(VERSION 3.10)

project(Common LANGUAGES CXX)

add_library(common
    "src/dtos/DeviceState.cpp"
    "src/dtos/SensorData.cpp"
    "src/utils/Logger.cpp"
    "include/dtos/DeviceState.h"
    "include/dtos/SensorData.h"
    "include/interfaces/IDeviceController.h"
    "include/interfaces/ISensorReader.h"
    "include/utils/Logger.h"
)

target_include_directories(common
    PUBLIC
        include
)

target_link_libraries(common
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        Qt6::Gui
        Qt6::Mqtt
)

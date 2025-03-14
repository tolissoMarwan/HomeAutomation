cmake_minimum_required(VERSION 3.10)

project(Data LANGUAGES CXX)

add_library(data
    "src/MqttService.cpp"
    "src/SensorRepository.cpp"
    "src/parsers/MqttMessageParser.cpp"
    "include/MqttService.h"
    "include/SensorRepository.h"
    "include/parsers/MqttMessageParser.h"
)

target_include_directories(data
    PUBLIC
        include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/common/include
)

target_link_libraries(data
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        Qt6::Gui
        Qt6::Mqtt
    PUBLIC
        common
)

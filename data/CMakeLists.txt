cmake_minimum_required(VERSION 3.10)

project(Data)

include_directories(${DATA_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)

find_package(Qt6 COMPONENTS Core Mqtt Sql REQUIRED)

file(GLOB DATA_SOURCES
    "src/*.cpp"
    "src/**/*.cpp"
    "include/*.h"
    "include/**/*.h"
)
add_library(data ${DATA_SOURCES})
add_library(data_implementation INTERFACE)
target_include_directories(data_implementation INTERFACE ${DATA_INCLUDE_DIR})

target_link_libraries(data_implementation INTERFACE data_implementation)
target_link_libraries(data
    Qt6::Core
    Qt6::Mqtt
    Qt6::Sql
    common
)

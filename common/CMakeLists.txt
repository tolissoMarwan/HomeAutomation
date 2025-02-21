cmake_minimum_required(VERSION 3.10)

project(Common)

include_directories(${COMMON_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Qt6 COMPONENTS Core REQUIRED)

file(GLOB COMMON_SOURCES
    "src/**/*.cpp"
    "include/**/*.h"
)
add_library(common ${COMMON_SOURCES})
add_library(common_implementation INTERFACE)
target_include_directories(common_implementation INTERFACE ${COMMON_INCLUDE_DIR})

target_link_libraries(common_implementation INTERFACE common_implementation)
target_link_libraries(common
    Qt6::Core

)

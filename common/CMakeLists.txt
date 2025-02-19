# Shared interfaces and utilities
project(Common LANGUAGES CXX)

add_library(Common
)

target_include_directories(Common INTERFACE
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

file(GLOB COMMON_SOURCES
  "src/**/*.cpp"
  "include/**/*.h"
)

target_sources(Common PRIVATE ${COMMON_SOURCES})

target_link_libraries(Common
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
)

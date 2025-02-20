qt_add_library(common STATIC
    src/dtos/DeviceState.cpp
    src/dtos/SensorData.cpp
    src/utils/logger.cpp

    include/dtos/DeviceState.h
    include/dtos/SensorData.h
    include/interfaces/IDeviceController.h
    include/interfaces/ISensorReader.h
    include/utils/Logger.h
)
target_link_libraries(common
    PRIVATE
        Qt6::Core
        Qt6::Mqtt
)

target_include_directories(common PUBLIC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # Expose headers to other projects
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src  # For internal includes
)


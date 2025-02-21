# Root CMakeLists.txt (home-automation/CMakeLists.txt)

cmake_minimum_required(VERSION 3.10)

project(HomeAutomation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Mqtt Widgets Network Sql Svg Test REQUIRED) # Add Sql, Test, and other modules as needed

# Subdirectories - Each has its own CMakeLists.txt
add_subdirectory(common)
add_subdirectory(data)
# add_subdirectory(logic)
# add_subdirectory(ui)
# add_subdirectory(tests) # Optional, if you want to build tests by default

# Resources (handled in the root CMakeLists.txt, but a .qrc file is preferable)
# set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
# qt6_add_resources(HOME_AUTOMATION_RESOURCES
#     ${RESOURCES_DIR}/styles/darktheme.qss
#     ${RESOURCES_DIR}/icons/light_on.png
#     ${RESOURCES_DIR}/icons/light_off.png
# )


# Target (executable) - Link against the UI and other libraries
# add_executable(home_automation main.cpp)  # Target name

# target_link_libraries(home_automation
#     Qt6::Core
#     Qt6::Gui
#     Qt6::Widgets
#     Qt6::Network
#     Qt6::Sql
#     Qt6::Svg
#     common  # The library target from common/CMakeLists.txt
#     data    # The library target from data/CMakeLists.txt
#     #logic   # The library target from logic/CMakeLists.txt
#     #ui      # The library target from ui/CMakeLists.txt
#     ${HOME_AUTOMATION_RESOURCES}
# )

# install(TARGETS home_automation DESTINATION bin)

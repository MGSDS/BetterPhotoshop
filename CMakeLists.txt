cmake_minimum_required(VERSION 3.18)

project(cg22_project_13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

add_executable(cg22_project_13)
add_subdirectory(Source)

target_link_libraries(cg22_project_13
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

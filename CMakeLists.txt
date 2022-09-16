cmake_minimum_required(VERSION 3.18)

project(Photoshop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

add_executable(Photoshop)
add_subdirectory(Source)

add_subdirectory(Deps/spdlog)
target_compile_definitions(Photoshop PUBLIC SPDLOG_COMPILED_LIB)

target_link_libraries(Photoshop PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
    spdlog
)

target_include_directories(Photoshop PUBLIC
    Source
    Deps/spdlog/include
)

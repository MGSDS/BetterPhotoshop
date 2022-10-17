cmake_minimum_required(VERSION 3.15)

project(Photoshop)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type was specified, building Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (MSVC)
    message(FATAL_ERROR "MSVC builds are currently not supported!")
endif()

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
add_subdirectory(Resources)
add_subdirectory(Source)

add_subdirectory(Deps/spdlog)
target_compile_definitions(Photoshop PUBLIC SPDLOG_COMPILED_LIB)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MINGW)
    if(NOT MINGW)
        target_compile_options(Photoshop PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(Photoshop PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    else()
        target_compile_options(Photoshop PRIVATE -fno-omit-frame-pointer)
        target_link_options(Photoshop PRIVATE -fno-omit-frame-pointer)
    endif()
endif()

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

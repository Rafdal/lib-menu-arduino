cmake_minimum_required(VERSION 3.20)

project(lib_menu_arduino LANGUAGES CXX)

option(LIB_MENU_USE_PC_STUB "Include the desktop Arduino compatibility shim" ON)
option(LIB_MENU_BUILD_CONSOLE "Build the interactive console harness" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

add_library(menu_arduino STATIC
    src/Menu.cpp
    src/MenuBase.cpp
    src/MessageBox.cpp
    src/NumControl.cpp
    src/NumForm.cpp
)

target_include_directories(menu_arduino
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
    $<$<BOOL:${LIB_MENU_USE_PC_STUB}>:${PROJECT_SOURCE_DIR}/pc-arduino-stub>
)

target_compile_features(menu_arduino PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(menu_arduino PRIVATE /W4 /permissive-)
else()
    target_compile_options(menu_arduino PRIVATE -Wall -Wextra -pedantic)
endif()

if (LIB_MENU_USE_PC_STUB)
    add_library(arduino_pc_stub STATIC pc-arduino-stub/Arduino.cpp)
    target_include_directories(arduino_pc_stub PUBLIC ${PROJECT_SOURCE_DIR}/pc-arduino-stub)

    if (MSVC)
        target_compile_options(arduino_pc_stub PRIVATE /W4 /permissive-)
    else()
        target_compile_options(arduino_pc_stub PRIVATE -Wall -Wextra -pedantic)
    endif()

    target_link_libraries(menu_arduino PUBLIC arduino_pc_stub)
else()
    message(STATUS "LIB_MENU_USE_PC_STUB=OFF - make sure your toolchain provides <Arduino.h>")
endif()

if (LIB_MENU_BUILD_CONSOLE)
    add_executable(menu_console
        tests/menu_console.cpp
    )

    target_link_libraries(menu_console PRIVATE menu_arduino Threads::Threads)

    if (MSVC)
        target_compile_options(menu_console PRIVATE /W4 /permissive-)
    else()
        target_compile_options(menu_console PRIVATE -Wall -Wextra -pedantic)
    endif()
endif()

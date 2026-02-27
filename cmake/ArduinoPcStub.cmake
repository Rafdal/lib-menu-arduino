include(CMakeParseArguments)

function(add_arduino_pc_stub)
    set(options)
    set(oneValueArgs TARGET_NAME ROOT_DIR)
    set(multiValueArgs EXTRA_SOURCES)
    cmake_parse_arguments(STUB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT STUB_TARGET_NAME)
        message(FATAL_ERROR "add_arduino_pc_stub requires TARGET_NAME")
    endif()

    if (NOT STUB_ROOT_DIR)
        message(FATAL_ERROR "add_arduino_pc_stub requires ROOT_DIR")
    endif()

    add_library(${STUB_TARGET_NAME} STATIC
        ${STUB_ROOT_DIR}/Arduino.cpp
        ${STUB_EXTRA_SOURCES}
    )

    target_include_directories(${STUB_TARGET_NAME} PUBLIC ${STUB_ROOT_DIR})

    if (MSVC)
        target_compile_options(${STUB_TARGET_NAME} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${STUB_TARGET_NAME} PRIVATE -Wall -Wextra -pedantic)
    endif()
endfunction()

function(link_arduino_pc_stub)
    set(options)
    set(oneValueArgs TARGET STUB_TARGET VISIBILITY)
    cmake_parse_arguments(LINK "${options}" "${oneValueArgs}" "" ${ARGN})

    if (NOT LINK_TARGET)
        message(FATAL_ERROR "link_arduino_pc_stub requires TARGET")
    endif()

    if (NOT LINK_STUB_TARGET)
        message(FATAL_ERROR "link_arduino_pc_stub requires STUB_TARGET")
    endif()

    if (NOT LINK_VISIBILITY)
        set(LINK_VISIBILITY PUBLIC)
    endif()

    target_link_libraries(${LINK_TARGET} ${LINK_VISIBILITY} ${LINK_STUB_TARGET})
endfunction()

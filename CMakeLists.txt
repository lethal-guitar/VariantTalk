cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(VariantTalk)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.4 COMPONENTS graphics window system REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(MSVC)
    add_compile_options(
        /MP
        /W4
        /WX
        /wd4503 # Decorated name length exceeded
        /wd4800 # Forcing value to bool
    )
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -Werror
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-padded
        -Wno-switch-enum

        -fcolor-diagnostics
    )
elseif(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(
        -Werror
        -Wall
        -Wextra
        -pedantic
    )
else()
    message(FATAL_ERROR "Unrecognized compiler")
endif()

add_subdirectory(event-handling)
add_subdirectory(lang-vm)
add_subdirectory(state-machine)

cmake_minimum_required(VERSION 3.24)

project(
    silk
    VERSION 0.1.0
    DESCRIPTION "A learning ground for HFT C++"
    LANGUAGES CXX
)

# ---- Global settings ----

# Only apply project-wide defaults (C++ standard, folders, testing) when this is
# the top-level project, so silk stays well-behaved if consumed via add_subdirectory.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Generate compile_commands.json for editors / clangd / tooling.
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Group targets nicely in IDEs.
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Default to a Release build if the user did not pick one.
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            Debug Release RelWithDebInfo MinSizeRel)
    endif()
endif()

# ---- Options ----

option(SILK_BUILD_TESTS "Build the unit tests" ON)
option(SILK_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# ---- Interface target carrying common warning flags ----

add_library(slk_warnings INTERFACE)
add_library(slk::warnings ALIAS slk_warnings)

if(MSVC)
    target_compile_options(slk_warnings INTERFACE /W4 /permissive-)
    if(SILK_WARNINGS_AS_ERRORS)
        target_compile_options(slk_warnings INTERFACE /WX)
    endif()
else()
    target_compile_options(slk_warnings INTERFACE
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    if(SILK_WARNINGS_AS_ERRORS)
        target_compile_options(slk_warnings INTERFACE -Werror)
    endif()
endif()

# ---- Subdirectories ----

add_subdirectory(src)
add_subdirectory(app)

if(SILK_BUILD_TESTS AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(tests)
endif()

# Unit tests, backed by GoogleTest pulled in via FetchContent so no system
# install is required.

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    GIT_SHALLOW ON
)

# Don't let GoogleTest override our compiler/linker settings on Windows.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(silk_tests parser_test.cpp file_test.cpp)

target_link_libraries(silk_tests
    PRIVATE
        silk::parser
        GTest::gtest_main)

# Register the tests with CTest via GoogleTest's discovery helper.
include(GoogleTest)
gtest_discover_tests(silk_tests)

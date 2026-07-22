# Entry-point executable. Kept thin: it just links the silk library.

add_executable(silk main.cpp)

target_link_libraries(silk
    PRIVATE
        slk::parser
        slk::warnings)

target_include_directories(silk
    PUBLIC ${CMAKE_SOURCE_DIR}/src )

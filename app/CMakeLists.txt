# Entry-point executable. Kept thin: it just links the slk library.

add_executable(silk main.cpp)

target_link_libraries(silk
    PRIVATE
        slk::io
        slk::parser
        slk::warnings)


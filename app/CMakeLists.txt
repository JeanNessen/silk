# Entry-point executable. Kept thin: it just links the silk library.

add_executable(silk main.cpp)

target_link_libraries(silk
    PRIVATE
        silk::lib
        silk::warnings)

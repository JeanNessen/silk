add_library(slk_io file.cpp)
add_library(slk::io ALIAS slk_io)

target_compile_features(slk_io PUBLIC cxx_std_23)

target_link_libraries(slk_io PRIVATE slk::core)

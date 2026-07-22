add_library(slk_parser parser.cpp)
add_library(slk::parser ALIAS slk_parser)

target_compile_features(slk_parser PUBLIC cxx_std_23)

target_link_libraries(slk_parser PUBLIC slk::io slk::core)

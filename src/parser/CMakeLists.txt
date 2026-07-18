add_library(silk_parser parser.cpp utils.cpp file.cpp)
add_library(silk::parser ALIAS silk_parser)

target_include_directories(silk_parser
    PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_features(silk_parser PUBLIC cxx_std_23)

target_compile_definitions(silk_parser
    PUBLIC SILK_VERSION="${PROJECT_VERSION}")

target_link_libraries(silk_parser PRIVATE silk::warnings)

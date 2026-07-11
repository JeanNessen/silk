# The core silk library. Real order-book code will live here; for now it only
# carries the project version so the scaffolding compiles and links end to end.

add_library(silk_lib version.cpp)
add_library(silk::lib ALIAS silk_lib)

target_include_directories(silk_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_features(silk_lib PUBLIC cxx_std_23)

target_compile_definitions(silk_lib
    PUBLIC SILK_VERSION="${PROJECT_VERSION}")

target_link_libraries(silk_lib PRIVATE silk::warnings)

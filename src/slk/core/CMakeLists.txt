add_library(slk_core INTERFACE)
add_library(slk::core ALIAS slk_core)

target_include_directories(slk_core INTERFACE ${CMAKE_SOURCE_DIR}/src )
target_compile_features(slk_core INTERFACE cxx_std_23)

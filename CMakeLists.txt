cmake_minimum_required(VERSION 3.15)

project(ikun_gui CXX)

# test
option(IKUN_GUI_BUILD_TEST "" OFF)

# skia
set(SKIA_PATH $ENV{SKIA_PATH})

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IKUN_GUI_BUILD_TEST ON)
endif ()

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall)

add_subdirectory(src)

if (IKUN_GUI_BUILD_TEST)
    link_libraries(ikun_gui)
    add_executable(count examples/main.cpp)
    add_executable(skia_HelloWorld examples/HelloWorld.cpp)

    get_target_property(skia_compile_defs skia COMPILE_DEFINITIONS)
    set_target_properties(skia_HelloWorld PROPERTIES COMPILE_DEFINITIONS "${skia_compile_defs}")
endif ()

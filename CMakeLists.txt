cmake_minimum_required(VERSION 3.15)

project(ikun-gui CXX C)

# examples
option(IKUN_GUI_BUILD_EXAMPLES "" OFF)

# skia
set(SKIA_PATH $ENV{SKIA_PATH})
if (NOT SKIA_PATH)
    set(SKIA_PATH ${CMAKE_CURRENT_LIST_DIR}/thirdparty/skia)
endif ()

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IKUN_GUI_BUILD_EXAMPLES ON)
endif ()

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall)

add_subdirectory(src)

if (IKUN_GUI_BUILD_EXAMPLES)
    add_executable(skia_HelloWorld examples/HelloWorld.cpp)
    target_link_libraries(skia_HelloWorld PRIVATE ikun_gui)
    get_target_property(ikun_gui_compile_defs ikun_gui COMPILE_DEFINITIONS)
    target_compile_definitions(skia_HelloWorld PUBLIC ${ikun_gui})

    add_executable(counter examples/counter.cpp)
    target_link_libraries(counter PRIVATE ikun_gui)
    get_target_property(ikun_gui_compile_defs ikun_gui COMPILE_DEFINITIONS)
    target_compile_definitions(counter PUBLIC ${ikun_gui})
endif ()

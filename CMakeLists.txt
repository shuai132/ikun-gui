cmake_minimum_required(VERSION 3.15)

project(ikun-gui CXX C)

include(cmake/common.cmake)

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
    add_executable(counter examples/counter.cpp)
    target_link_libraries_with_definitions(counter ikun_gui)
endif ()

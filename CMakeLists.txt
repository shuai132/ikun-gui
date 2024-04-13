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
    if (CMAKE_BUILD_TYPE MATCHES "Release")
        set(BUILD_APP_FLAG WIN32 MACOSX_BUNDLE)
    endif ()

    add_executable(counter ${BUILD_APP_FLAG} examples/counter.cpp)
    target_link_libraries_with_definitions(counter ikun_gui)

    add_executable(counter_component ${BUILD_APP_FLAG} examples/counter_component.cpp)
    target_link_libraries_with_definitions(counter_component ikun_gui)

    add_executable(image ${BUILD_APP_FLAG} examples/image.cpp)
    target_link_libraries_with_definitions(image ikun_gui)

    add_executable(textview ${BUILD_APP_FLAG} examples/textview.cpp)
    target_link_libraries_with_definitions(textview ikun_gui)

    add_executable(code_editor ${BUILD_APP_FLAG} examples/code_editor.cpp)
    target_link_libraries_with_definitions(code_editor ikun_gui)
endif ()

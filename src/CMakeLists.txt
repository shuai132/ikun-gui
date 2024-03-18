cmake_minimum_required(VERSION 3.15)

file(GLOB SRCS render/*.cpp)

add_library(ikun_gui STATIC ${SRCS})

if (NOT SKIA_USE_STATIC_LIBS)
    if (NOT EXISTS ${SKIA_PATH}/out/config/CMakeLists.txt)
        message(STATUS "Generating skia cmake, please wait...")
        message(STATUS "1. git-sync-deps...")
        execute_process(
                COMMAND python tools/git-sync-deps
                WORKING_DIRECTORY ${SKIA_PATH}
        )
        message(STATUS "2. gn_to_cmake...")
        execute_process(
                COMMAND bin/gn gen out/config --ide=json --json-ide-script=../../gn/gn_to_cmake.py
                WORKING_DIRECTORY ${SKIA_PATH}
        )
        message(STATUS "Generating skia cmake, done!")
    endif ()

    add_subdirectory(${SKIA_PATH}/out/config skia EXCLUDE_FROM_ALL)
    target_link_libraries(ikun_gui PUBLIC skia flags sk_app tool_utils gpu_tool_utils modules__svg tools__window)
else ()
    target_link_directories(ikun_gui PUBLIC ${SKIA_PATH}/out/config/cmake-build-debug)
    target_link_libraries(ikun_gui PUBLIC skia flags sk_app tool_utils gpu_tool_utils modules__svg tools__window)
endif ()

target_include_directories(ikun_gui PUBLIC skia ${SKIA_PATH})

get_target_property(skia_compile_defs skia COMPILE_DEFINITIONS)
target_compile_definitions(ikun_gui PUBLIC ${skia_compile_defs})

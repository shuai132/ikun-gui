cmake_minimum_required(VERSION 3.15)

option(ENABLE_MORE_GPU_BACKEND "" OFF)

file(GLOB src_app app/*.cpp)
file(GLOB_RECURSE src_core core/*.cpp)
file(GLOB_RECURSE src_render render/*.cpp)
file(GLOB_RECURSE src_components components/*.cpp)

add_library(ikun_gui STATIC
        ${src_app}
        ${src_core}
        ${src_render}
        ${src_components}
)

if (NOT SKIA_USE_STATIC_LIBS)
    set(SKIA_BUILD_ARGS)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND SKIA_BUILD_ARGS " is_official_build=false")
        string(APPEND SKIA_BUILD_ARGS " is_debug=false")
        string(APPEND SKIA_BUILD_ARGS " cc_wrapper=\"ccache\"")
    else ()
        string(APPEND SKIA_BUILD_ARGS " is_official_build=false")
        string(APPEND SKIA_BUILD_ARGS " is_debug=true")
        string(APPEND SKIA_BUILD_ARGS " cc_wrapper=\"ccache\"")
    endif ()

    if (APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        string(APPEND SKIA_BUILD_ARGS " target_cpu=\"arm64\"")
    endif ()

    string(APPEND SKIA_BUILD_ARGS " skia_use_gl=true")
    if (ENABLE_MORE_GPU_BACKEND)
        string(APPEND SKIA_BUILD_ARGS " skia_use_vulkan=true")
        if (APPLE)
            string(APPEND SKIA_BUILD_ARGS " skia_use_metal=true")
        endif ()
    endif ()

    message(STATUS "Generating skia cmake, please wait...")
    message(STATUS "1. git-sync-deps...")
    if (NOT EXISTS ${SKIA_PATH}/out/${CMAKE_BUILD_TYPE}/CMakeLists.txt)
        execute_process(
                COMMAND python tools/git-sync-deps
                WORKING_DIRECTORY ${SKIA_PATH}
        )
    else ()
        message(STATUS "skip")
    endif ()

    message(STATUS "2. gn_to_cmake...")
    set(GN_TO_CMAKE_CMD "bin/gn gen out/${CMAKE_BUILD_TYPE} --args='${SKIA_BUILD_ARGS}' --ide=json --json-ide-script=../../gn/gn_to_cmake.py")
    message(STATUS ${GN_TO_CMAKE_CMD})
    execute_process(
            COMMAND bash -c "${GN_TO_CMAKE_CMD}"
            WORKING_DIRECTORY ${SKIA_PATH}
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
    )
    message(${output})
    message(STATUS "Generating skia cmake, done!")

    add_subdirectory(${SKIA_PATH}/out/${CMAKE_BUILD_TYPE} skia EXCLUDE_FROM_ALL)
    target_link_libraries(ikun_gui PUBLIC skia flags tool_utils gpu_tool_utils modules__svg tools__window)
else ()
    message(FATAL_ERROR "not support yet")
endif ()

add_subdirectory(app)
target_link_libraries(ikun_gui PUBLIC ikun_gui_app)

add_subdirectory(../thirdparty/yoga/yoga yoga)
target_link_libraries(ikun_gui PUBLIC yogacore)

add_subdirectory(../thirdparty/fmt fmt)
target_link_libraries(ikun_gui PUBLIC fmt)

target_include_directories(ikun_gui PUBLIC
        .
        ${SKIA_PATH}
)

target_link_libraries_with_definitions(ikun_gui PUBLIC skia)

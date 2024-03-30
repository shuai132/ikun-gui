cmake_minimum_required(VERSION 3.15)

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
    target_link_libraries(ikun_gui PUBLIC skia flags tool_utils gpu_tool_utils modules__svg tools__window)
else ()
    target_link_directories(ikun_gui PUBLIC ${SKIA_PATH}/out/config/cmake-build-debug)
    target_link_libraries(ikun_gui PUBLIC skia flags tool_utils gpu_tool_utils modules__svg tools__window)
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

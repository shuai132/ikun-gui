cmake_minimum_required(VERSION 3.15)

file(GLOB SRCS render/*.cpp)

add_library(ikun_gui STATIC ${SRCS})

if (NOT SKIA_USE_STATIC_LIBS)
    add_subdirectory(${SKIA_PATH}/out/config skia EXCLUDE_FROM_ALL)
    target_link_libraries(ikun_gui PUBLIC skia flags sk_app tool_utils gpu_tool_utils modules__svg tools__window)
else ()
    target_link_directories(ikun_gui PUBLIC ${SKIA_PATH}/out/config/cmake-build-debug)
    target_link_libraries(ikun_gui PUBLIC skia flags sk_app tool_utils gpu_tool_utils modules__svg tools__window)
endif ()
target_include_directories(ikun_gui PUBLIC skia ${SKIA_PATH})

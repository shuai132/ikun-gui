cmake_minimum_required(VERSION 3.15)

file(GLOB SRCS *.cpp)
file(GLOB SRCS_PLATFORM mac/*.cpp mac/*.mm)

add_library(ikun_gui_app STATIC ${SRCS} ${SRCS_PLATFORM})
target_link_libraries_with_definitions(ikun_gui_app PUBLIC skia)
target_include_directories(ikun_gui_app PUBLIC ${SKIA_PATH})

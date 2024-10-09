cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -pedantic -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS "-DNDEBUG -O3")

project(gljam VERSION 1.0)

set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

set(STB_IMAGE ${LIB}/stb_image)
set(IMGUI ${LIB}/imgui)
set(IMGUI_BACKENDS ${LIB}/${IMGUI}/backends)
set(GLM ${LIB}/glm)
set(FLECS ${LIB}/flecs)
set(GLAD ${LIB}/glad)

## MY FILES
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${FLECS}/flecs.c ${IMGUI}/*.cpp ${IMGUI_BACKENDS}/*.cpp ${GLAD}/src/glad.c ${STB_IMAGE}/stb_image.cpp)
add_executable(gljam ${CMAKE_CURRENT_SOURCE_DIR}/src/gljam.cpp ${SRC_FILES})
target_include_directories(gljam PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLAD}/include/ ${STB_IMAGE} ${IMGUI} ${IMGUI_BACKEND} ${LIB} ${FLECS} )

add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)

find_library(ASSIMP assimp)
target_link_libraries(gljam PUBLIC ${ASSIMP})

find_library(PTHREAD pthread)
target_link_libraries(gljam PUBLIC ${PTHREAD})

find_library(XRANDR Xrandr)
target_link_libraries(gljam PUBLIC ${XRANDR})

find_library(DL dl)
target_link_libraries(gljam PUBLIC ${DL})

find_library(GLFW glfw)
target_link_libraries(gljam PUBLIC ${GLFW})

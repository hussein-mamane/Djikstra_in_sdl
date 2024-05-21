cmake_minimum_required(VERSION 3.22)
project(algo_in_sdl)

set(CMAKE_CXX_STANDARD 11)

add_executable(algo_in_sdl
        main.cpp
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)

set(SDL2_IMAGE_PATH "SDL2Image/x86_64-w64-mingw32")
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARY})
cmake_minimum_required(VERSION 3.4)

project(level_gen)

link_directories("C:\\mingw64-gcc6.1.0\\lib")
include_directories("C:\\mingw64-gcc6.1.0\\include")
SET(CMAKE_EXE_LINKER_FLAGS "-lsfml-graphics -lsfml-window -lsfml-system")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -Wall -Wextra")

file(GLOB all_src "*.hpp" "*.cpp")

add_executable(level_gen ${all_src})

target_link_libraries(level_gen sfml-graphics sfml-window sfml-system sfml-network)

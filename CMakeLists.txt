cmake_minimum_required(VERSION 3.5.1)

project(minesweeper)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES Game.cpp graphics.cpp main.cpp)

add_executable(minesweeper ${SOURCES})

set(LIBS sfml-window sfml-graphics sfml-system)
target_link_libraries(minesweeper ${LIBS})

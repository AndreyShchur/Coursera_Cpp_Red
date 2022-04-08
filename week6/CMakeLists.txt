cmake_minimum_required(VERSION 3.21)
set(PROJ_NAME week6)
project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../week2)
add_compile_options(-pthread)


set(FINAL_TASK main.cpp parse.cpp search_server.cpp)


add_executable(${PROJ_NAME} ${FINAL_TASK})

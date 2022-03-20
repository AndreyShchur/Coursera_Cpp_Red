cmake_minimum_required(VERSION 3.21)
set(PROJ_NAME week4)
project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../week2)
#add_compile_options(-O2)


set(AIRPORTS_COUNTER airport_counter.cpp)
set(TEXT_EDITOR text_editor.cpp)
set(SERVER_STATS server_stats.cpp stats.cpp)


add_executable(${PROJ_NAME} ${SERVER_STATS})

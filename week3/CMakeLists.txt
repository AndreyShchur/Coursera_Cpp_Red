cmake_minimum_required(VERSION 3.21)
project(week3)

set(CMAKE_CXX_STANDARD 14)

set(OBJ_POOL object_pool.cpp)
set(SIMPLE_VECTOR simple_vector.cpp)
set(LINKED_LIST linked_list.cpp)

add_executable(week3 ${LINKED_LIST})

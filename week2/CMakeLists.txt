cmake_minimum_required(VERSION 3.21)
project(week2)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_SLOW slow.cpp)
set(EXECUTABLE_FAST fast.cpp)
set(EXECUTABLE_BOOKING booking.cpp)

add_executable(week2 ${EXECUTABLE_BOOKING})

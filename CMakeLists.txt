cmake_minimum_required(VERSION 3.3)
project(CSCI2312PA2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Point.h Cluster.h KMeans.h Exceptions.cpp Exceptions.h)
add_executable(CSCI2312PA2 ${SOURCE_FILES})

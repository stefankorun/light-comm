cmake_minimum_required(VERSION 2.8.4)
project(light_comm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/emiter/emiter.c
    src/emiter/emiter.h
    src/cr_startup_lpc17.c
    src/main.c)

add_executable(light_comm ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.12)
project(interface C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(interface
        exercise.c
        exercise.h
        input.h
        what.h )

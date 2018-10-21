cmake_minimum_required(VERSION 3.12)
project(ReseauNeurone C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ReseauNeurone
        learning.c
        learning.h
        main.c)

target_link_libraries(ReseauNeurone m)
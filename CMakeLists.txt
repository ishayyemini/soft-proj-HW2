cmake_minimum_required(VERSION 3.28)
project(HW2 C)

set(CMAKE_C_STANDARD 90)

add_executable(HW2 kmeanslib.h kmeansmodule.c kmeans.c)

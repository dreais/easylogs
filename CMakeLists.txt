cmake_minimum_required(VERSION 3.12)
project(easylogs C)

set(CMAKE_C_STANDARD 11)

add_library(easylogs STATIC easylogs.c easylogs.h)
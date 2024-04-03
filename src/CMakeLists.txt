# CMake minimum version
cmake_minimum_required(VERSION 3.22)

# Name of the project
project(source)

# ----------------------------- Subdirectories ----------------------------- #

add_subdirectory(main)
add_subdirectory(util)
add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(models)
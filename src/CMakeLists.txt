# CMake minimum version
cmake_minimum_required(VERSION 3.22)

# Name of the project
project(source)

# -------------------------------- Variables -------------------------------- #

# set(CMAKE_CXX_CLANG_TIDY
#   clang-tidy;
#   -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;
# )

# ----------------------------- Subdirectories ----------------------------- #

add_subdirectory(main)
add_subdirectory(util)
add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(models)
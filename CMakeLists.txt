# CMake minimum version
cmake_minimum_required(VERSION 3.22)

# Set project basic information
project(root
  VERSION 0.2.1
  DESCRIPTION "Exact algorithm for the optimization of the use of the astronomical instrument EMIR"
  HOMEPAGE_URL "https://github.com/gianluisdiana/OpsCplex"
  LANGUAGES CXX
)

# ----------------------------- Subdirectories ----------------------------- #

add_subdirectory(lib)
add_subdirectory(src)
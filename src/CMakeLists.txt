# CMake minimum version
cmake_minimum_required(VERSION 3.22)

# Set project basic information
project(emir_cplex
  VERSION 0.1.0
  DESCRIPTION "Exact algorithm for the optimization of the use of the astronomical instrument EMIR"
  HOMEPAGE_URL "https://github.com/gianluisdiana/OpsCplex"
  LANGUAGES CXX
)

# ----------------------------- Subdirectories ----------------------------- #

add_subdirectory(main)
add_subdirectory(util)
add_subdirectory(json)
add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(models)
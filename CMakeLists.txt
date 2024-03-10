# CMake minimum version
cmake_minimum_required(VERSION 3.22)

# Set project basic information
project(root
  VERSION 0.2.1
  DESCRIPTION "Exact algorithm for the optimization of the use of the astronomical instrument EMIR"
  HOMEPAGE_URL "https://github.com/gianluisdiana/OpsCplex"
  LANGUAGES CXX
)

# --------------------------------- Options --------------------------------- #

set(CMAKE_CXX_STANDARD 20)

# ----------------------------- Subdirectories ------------------------------ #

add_subdirectory(src)

# Only add the tests directory if the BUILD_TESTS option is turned on
# cmake -DBUILD_TESTS=ON ..
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
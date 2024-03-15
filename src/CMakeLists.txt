# CMake minimum version
cmake_minimum_required(VERSION 3.22)

# Name of the project
project(source)

# ------------------------------- Clang tidy ------------------------------- #

find_program(CLANG_TIDY_COMMAND NAMES "clang-tidy")
if(CLANG_TIDY_COMMAND)
  set(CMAKE_CXX_CLANG_TIDY
    ${CLANG_TIDY_COMMAND};
    -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;
  )
endif()

# ----------------------------- Subdirectories ----------------------------- #

add_subdirectory(main)
add_subdirectory(util)
add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(models)
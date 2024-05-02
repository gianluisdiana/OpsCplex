cmake_minimum_required(VERSION 3.22)
# Set the project name
project(util)

# ------------------------------ Source files ------------------------------- #

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
  src/functions.cpp
)

# Add an alias to the project
add_library(sub::util ALIAS ${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}
  sub::models
)

# ------------------------------ Header files ------------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
  PUBLIC include
)
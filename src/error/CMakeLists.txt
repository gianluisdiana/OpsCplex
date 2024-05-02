# Set the project name
project(error)

add_library(${PROJECT_NAME} INTERFACE)

# Add an alias to the project
add_library(sub::error ALIAS ${PROJECT_NAME})

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME} INTERFACE .)
# Set the project name
project(util)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
    src/timer_t.cpp
)

# Add an alias to the project
add_library(sub::gomautil ALIAS ${PROJECT_NAME})

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
    PUBLIC include
)
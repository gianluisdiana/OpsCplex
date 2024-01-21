# Set the project name
project(json)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
    src/json.cpp
)

# Add an alias to the library
add_library(sub::json ALIAS ${PROJECT_NAME})

# -------------------------------- Header files -------------------------------- #

# Add a library with the above sources
target_include_directories(${PROJECT_NAME}
    PUBLIC include
)
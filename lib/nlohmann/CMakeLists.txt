# Set the project name
project(nlohmann_json)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
  src/json.cpp
)

# Add an alias to the project
add_library(lib::nlohmann_json ALIAS ${PROJECT_NAME})

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
  PUBLIC include
)
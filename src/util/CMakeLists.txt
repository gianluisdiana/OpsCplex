cmake_minimum_required(VERSION 3.22)
# Set the project name
project(util)

# ------------------------------ Source files ------------------------------- #

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
  src/functions.cpp
  src/timer_t.cpp
)

# Add an alias to the project
add_library(sub::gomautil ALIAS ${PROJECT_NAME})

# ---------------------------- Json Dependencies ---------------------------- #

include(FetchContent)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC nlohmann_json::nlohmann_json
)


# ------------------------------ Header files ------------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
  PUBLIC include
)
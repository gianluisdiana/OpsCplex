# Set the project name
project(input)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
    src/arc.cpp
    src/graph.cpp
    src/node.cpp
    src/ops_input.cpp
    src/ops_instance.cpp
)

# Add an alias to the project
add_library(sub::input ALIAS ${PROJECT_NAME})

# ---------------------------- Json Dependencies ---------------------------- #

include(FetchContent)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
  PUBLIC include
)

# Include the subprojects gomautil and json
target_link_libraries(${PROJECT_NAME}
  PUBLIC nlohmann_json::nlohmann_json
  sub::util
)
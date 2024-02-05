# Set the project name
project(input)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
    src/arc.cpp
    src/graph.cpp
    src/ops_input.cpp
    src/OPS_instance_t.cpp
)

# Add an alias to the project
add_library(sub::input ALIAS ${PROJECT_NAME})

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
    PUBLIC include
)

# Include the subprojects gomautil and json
target_link_libraries(${PROJECT_NAME}
    sub::gomautil
    sub::json
)
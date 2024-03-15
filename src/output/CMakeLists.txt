# Set the project name
project(output)

# Add a library with the above sources
add_library(${PROJECT_NAME}
    src/ops_output.cpp
)

# Add an alias to the project
add_library(sub::output ALIAS ${PROJECT_NAME})

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Include the subprojects gomautil and input
target_link_libraries(${PROJECT_NAME}
    sub::input
)
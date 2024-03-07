# CMake minimum version
cmake_minimum_required (VERSION 3.22)

# Set the project name
project(main)

# List of sources specified for a target
set(SOURCES
    src/solvers.cpp
    src/main.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# ----------------------------- Header files ----------------------------- #

# Add the include directories to access the headers
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Link the static libraries form the subprojects and cplex
target_link_libraries(${PROJECT_NAME}
    sub::models
)
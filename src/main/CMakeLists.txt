# CMake minimum version
cmake_minimum_required (VERSION 3.22)

# Set the project name
project(main)

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# ----------------------------- Header files ----------------------------- #

# Add the include directories to access the headers
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Link the static libraries form the subprojects and cplex
target_link_libraries(${PROJECT_NAME}
    sub::models
)
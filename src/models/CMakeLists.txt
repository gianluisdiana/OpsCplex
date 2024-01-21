# Set the project name
project(solver)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
    src/OPS_bc1.cpp
)

# Add an alias to the project
add_library(sub::solver ALIAS ${PROJECT_NAME})

# ----------------------- Path variable (for cplex) ------------------------ #

set (CPX_PATH /home/gianluisdiana/dev/external_libraries/CPLEX_Studio2211)

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CPX_PATH}/cplex/include/
    PRIVATE ${CPX_PATH}/concert/include/
)

# Include the subprojects gomautil, input and output
target_link_libraries(${PROJECT_NAME}
    sub::gomautil
    sub::input
    sub::output
)
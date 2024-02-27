# Set the project name
project(models)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
    src/ops_bc.cpp
)

# Add an alias to the project
add_library(sub::models ALIAS ${PROJECT_NAME})

# ----------------------- Path variable (for cplex) ------------------------ #

# Set the path to the cplex library
set(CPX_PATH ${CMAKE_SOURCE_DIR}/lib/CplexStudio2211/)

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CPX_PATH}/cplex/include
    PRIVATE ${CPX_PATH}/concert/include
)

# Include the subprojects gomautil, input and output
target_link_libraries(${PROJECT_NAME}
    sub::output
)
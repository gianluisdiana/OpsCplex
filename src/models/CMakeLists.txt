# Set the project name
project(models)

# Specify the files to compile
add_library(${PROJECT_NAME} STATIC
    src/ops_bc.cpp
    src/ops_solver.cpp
)

# Add an alias to the project
add_library(sub::models ALIAS ${PROJECT_NAME})

# ----------------------- Path variable (for cplex) ------------------------ #

# Set the path to the cplex library
set(CPX_PATH ${CMAKE_SOURCE_DIR}/lib/CplexStudio2211/)
set(CPX_LIB_PATH ${CPX_PATH}/cplex/lib/x86-64_linux/static_pic/)
set(ILO_LIB_PATH ${CPX_PATH}/cplex/lib/x86-64_linux/static_pic/)
set(CON_LIB_PATH ${CPX_PATH}/concert/lib/x86-64_linux/static_pic/)

# -------------------------------- Libraries -------------------------------- #

# Help CMake find the static libraries from cplex
find_library(CPX_LIBRARY libcplex.a HINTS ${CPX_LIB_PATH})
find_library(ILO_LIBRARY libilocplex.a HINTS ${ILO_LIB_PATH})
find_library(CON_LIBRARY libconcert.a HINTS ${CON_LIB_PATH})

# ----------------------------- Header files ----------------------------- #

# Add the headers files of the project to the library
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${CPX_PATH}/cplex/include
    PUBLIC ${CPX_PATH}/concert/include
)

# Include the subprojects gomautil, input and output
target_link_libraries(${PROJECT_NAME}
    sub::output
    ${CON_LIBRARY}
    ${ILO_LIBRARY}
    ${CPX_LIBRARY}
)
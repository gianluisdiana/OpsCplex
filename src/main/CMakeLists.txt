# CMake minimum version
cmake_minimum_required (VERSION 3.22)

# Set the project name
project(main)

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# ----------------------- Path variables (for cplex) ------------------------ #

set(CPX_PATH ~/dev/external_libraries/CPLEX_Studio2211)
set(CPX_LIB_PATH ${CPX_PATH}/cplex/lib/x86-64_linux/static_pic/)
set(ILO_LIB_PATH ${CPX_PATH}/cplex/lib/x86-64_linux/static_pic/)
set(CON_LIB_PATH ${CPX_PATH}/concert/lib/x86-64_linux/static_pic/)

# -------------------------------- Libraries -------------------------------- #

# Help CMake find the static libraries from cplex
find_library(CPX_LIBRARY libcplex.a HINTS ${CPX_LIB_PATH})
find_library(ILO_LIBRARY libilocplex.a HINTS ${ILO_LIB_PATH})
find_library(CON_LIBRARY libconcert.a HINTS ${CON_LIB_PATH})

# Add the include directories to access the headers
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CPX_PATH}/cplex/include/
    PRIVATE ${CPX_PATH}/concert/include/
)

# Link the static libraries form the subprojects and cplex
target_link_libraries(${PROJECT_NAME}
    sub::gomautil
    sub::input
    sub::output
    sub::solver
    ${CON_LIBRARY}
    ${ILO_LIBRARY}
    ${CPX_LIBRARY}
)
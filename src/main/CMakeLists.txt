project(emir_cpx)

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the static library from subproject1 using its alias sub::lib1
# Link the header only library from subproject2 using its alias sub::lib2
# This will cause the include directories for that target to be added to this project

set (CPX_PATH /home/gianluisdiana/dev/external_libraries/CPLEX_Studio2211)
set (CPX_LIB_PATH ${CPX_PATH}/cplex/lib/x86-64_linux/static_pic/)
set (ILO_LIB_PATH ${CPX_PATH}/cplex/lib/x86-64_linux/static_pic/)
set (CON_LIB_PATH ${CPX_PATH}/concert/lib/x86-64_linux/static_pic/)

find_library(CPX_LIBRARY libcplex.a HINTS ${CPX_LIB_PATH})
find_library(ILO_LIBRARY libilocplex.a HINTS ${ILO_LIB_PATH})
find_library(CON_LIBRARY libconcert.a HINTS ${CON_LIB_PATH})

#message(CPX_LIBRARY="${CPX_LIBRARY}")
#message(CON_LIBRARY="${CON_LIBRARY}")
#message(ILO_LIBRARY="${ILO_LIBRARY}")

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CPX_PATH}/cplex/include/
    PRIVATE ${CPX_PATH}/concert/include/
) 


target_link_libraries(${PROJECT_NAME}
    sub::gomautil
    sub::input
    sub::output
    sub::solver
    ${CON_LIBRARY}
    ${ILO_LIBRARY}
    ${CPX_LIBRARY}
) 
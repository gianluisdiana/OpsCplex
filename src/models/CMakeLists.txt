# Set the project name
project (solver)

# --- INCLUDE CPX LIBRARY TO PROJECT
set (CPX_PATH /home/gianluisdiana/dev/external_libraries/CPLEX_Studio2211)

file(GLOB SOURCES
    "src/OPS_bc1.cpp"
)


# Add a library with the above sources
add_library(${PROJECT_NAME}
    ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    sub::gomautil
    sub::input
    sub::output
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CPX_PATH}/cplex/include/
    PRIVATE ${CPX_PATH}/concert/include/
)

add_library(sub::solver ALIAS ${PROJECT_NAME})
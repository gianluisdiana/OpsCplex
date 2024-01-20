# Set the project name
project (input)

# Add a library with the above sources
add_library(${PROJECT_NAME} 
    src/OPS_input_t.cpp
    src/OPS_instance_t.cpp
    )
    
add_library(sub::input ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    sub::gomautil
    sub::json
)

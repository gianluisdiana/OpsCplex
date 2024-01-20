# Set the project name
project (util)

file(GLOB SOURCES 
    "src/timer_t.cpp" 
)

# Add a library with the above sources

add_library(${PROJECT_NAME} 
    ${SOURCES})

add_library(sub::gomautil ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

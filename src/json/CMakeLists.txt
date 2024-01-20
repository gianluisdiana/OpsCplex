# Set the project name
project (json)

# --- INCLUDE CPX LIBRARY TO PROJECT
    
file(GLOB SOURCES 
    "src/json.cpp"
)


# Add a library with the above sources
add_library(${PROJECT_NAME} 
    ${SOURCES})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_library(sub::json ALIAS ${PROJECT_NAME})   
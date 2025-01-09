cmake_minimum_required(VERSION 3.31)

# Set C++ version to C++11 for this directory and below
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure flatc is available
find_program(FLATC flatc REQUIRED)

# Create a list of all flatBuffers schema file files to process
file(GLOB SCHEMA_FILES "${CMAKE_CURRENT_LIST_DIR}/*.fbs")

# Specify the output directory for the generated code
set(GENERATED_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# Add custom target to ensure code generation runs before compilation
add_custom_target(generate_flatbuffers_code ALL)

# Iterate over each file and add a custom command
# to generate C code from the FlatBuffers schema
foreach(file ${SCHEMA_FILES})
    add_custom_command(
        TARGET generate_flatbuffers_code PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GENERATED_DIR}"
        COMMAND "${FLATC}" -c --no-prefix --gen-all --gen-mutable --cpp-std c++11 -o "${GENERATED_DIR}" "${file}"
        COMMENT "Generating C code from FlatBuffers schemas"
    )
endforeach()

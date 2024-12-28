cmake_minimum_required(VERSION 3.21)

project(DemoProject VERSION 1.0.0 LANGUAGES C CXX)

# Set C standard to C99 for this directory and below
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set C++ version to C++17 for this directory and below
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Werror)
endif()

if(MSVC)
    add_compile_options(/WX)
endif()

# Ensure flatc is available
find_program(FLATC flatc REQUIRED)

# Define the FlatBuffers schema file
set(SCHEMA_FILES
    ${CMAKE_CURRENT_LIST_DIR}/client_message.fbs
    ${CMAKE_CURRENT_LIST_DIR}/server_message.fbs
)
# Create a list of all .fbs files to process
file(GLOB SCHEMA_FILES "${CMAKE_CURRENT_LIST_DIR}/*.fbs")

# Specify the output directory for the generated code
set(GENERATED_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# Add custom target to ensure code generation runs before compilation
add_custom_target(generate_flatbuffers_code ALL)

# Generate C code from the FlatBuffers schema
add_custom_command(
  TARGET generate_flatbuffers_code PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${GENERATED_DIR}"
  COMMAND "${FLATC}" -c --no-prefix --gen-all --gen-mutable --cpp-std c++0x -o "${GENERATED_DIR}" "${SCHEMA_FILES}"
  COMMENT "Generating C code from FlatBuffers schemas"
)

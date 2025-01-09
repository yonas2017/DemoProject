cmake_minimum_required(VERSION 3.31)

# Set C++ version to C++17 for this directory and below
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure Project to Use Google Test
find_package(GTest REQUIRED)

find_package(flatbuffers REQUIRED)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Add executable for your main code
add_executable(client_server_test
    tcp_test.cpp
    schema_test.cpp)

target_link_libraries(client_server_test 
    ${GTEST_LIBRARIES}
    flatbuffers)

target_include_directories(client_server_test PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/../inc)

target_compile_features(client_server_test PRIVATE cxx_std_17)

# Ensure the build test directory exists
set(BUILD_TEST_DIR "${CMAKE_BINARY_DIR}/test")
file(MAKE_DIRECTORY ${BUILD_TEST_DIR})

# Create a custom target to copy files after the build
add_custom_target(copy_files
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_LIST_DIR}/test_file" ${BUILD_TEST_DIR}/test_file
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_LIST_DIR}/script_test.sh" ${BUILD_TEST_DIR}/script_test.sh
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_LIST_DIR}/python_test.py" ${BUILD_TEST_DIR}/python_test.py
    COMMENT "Copying files to the build directory"
)

# Ensure that the 'copy_files' target runs after the executable is built
add_dependencies(client_server_test copy_files)

# Enable testing
enable_testing()

# Register the test with CTest
add_test(NAME TCPClientServerTest COMMAND client_server_test)
add_test(NAME DemoScriptTest COMMAND "./script_test.sh")
add_test(NAME PythonTest COMMAND ${Python3_EXECUTABLE} python_test.py)

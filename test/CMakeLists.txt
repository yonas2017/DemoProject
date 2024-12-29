cmake_minimum_required(VERSION 3.21)

# Set C++ version to C++17 for this directory and below
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure Project to Use Google Test
find_package(GTest REQUIRED)

# Add executable for your main code
add_executable(client_server_test
    tcp_test.cpp
    schema_test.cpp)

target_link_libraries(client_server_test ${GTEST_LIBRARIES})

target_include_directories(client_server_test PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/../inc)

# Enable testing
enable_testing()

# Register the test with CTest
add_test(NAME TCPClientServerTest COMMAND client_server_test)
add_test(NAME DemoScriptTest 
    COMMAND "${CMAKE_CURRENT_LIST_DIR}/script_test.sh"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

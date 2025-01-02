cmake_minimum_required(VERSION 3.31)

add_library(server STATIC
  server.h
  server.c
  tcp_server.h
  tcp_server.c
)

add_subdirectory(jobfiles)

# Ensure that the 'run_jobs' target runs after the library is built
add_dependencies(server run_jobs)

# Specify the include directories for this library
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_features(client PRIVATE c_std_99)

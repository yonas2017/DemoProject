cmake_minimum_required(VERSION 3.21)

add_library(server STATIC
  server.h
  server.c
  tcp_server.h
  tcp_server.c
)

# Specify the include directories for this library
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_features(client PRIVATE c_std_99)

cmake_minimum_required(VERSION 3.31)

add_library(client STATIC 
  client.h
  client.c
  pipe.h
  pipe.c
  tcp_client.h
  tcp_client.c
)

# Specify the include directories for this library
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_features(client PRIVATE c_std_99)
cmake_minimum_required(VERSION 3.31)

add_library(server STATIC
  server.h
  server.c
  tcp_server.h
  tcp_server.c
)

# Create a custom target to copy files after the build
add_custom_target(copy_jobfiles
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/jobfiles ${CMAKE_BINARY_DIR}/jobfiles
    COMMENT "Copying jobfiles to the build directory"
)

# Ensure that the 'copy_jobfiles' target runs after the ibrary is built
add_dependencies(server copy_jobfiles)

# Specify the include directories for this library
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_features(client PRIVATE c_std_99)

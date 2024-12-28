cmake_minimum_required(VERSION 3.21)

project(DemoProject VERSION 1.0.0 LANGUAGES C CXX)

# Set C standard to C99 for this directory and below
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Werror)
endif()

if(MSVC)
    add_compile_options(/WX)
endif()

# Enable clang-tidy if it's available
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" HINTS "/usr/bin" "/usr/local/bin")

if(CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    
    # Set the clang-tidy options you want
    set(CLANG_TIDY_OPTIONS
        -checks=-*,modernize-*,readability-*,clang-analyzer-*,performance-*
    )

    # Enable clang-tidy for every target in the project
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE};${CLANG_TIDY_OPTIONS}")

else()
    message(WARNING "clang-tidy not found, skipping static analysis.")
endif()

# Find clang-format
find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")
    
    # Define the list of source files to format. You can use `file(GLOB ...)` to collect files
    file(GLOB_RECURSE SOURCE_FILES
        src/client/*.c
        src/client/*.h
        src/server/*.c
        src/server/*.h
        test/*.c
        inc/*.h
    )

    # Add a custom target to format code
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format on all source files"
    )
else()
    message(WARNING "clang-format not found. Skipping formatting.")
endif()

# Find Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE}")

    # Add a custom target for documentation
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        COMMENT "Generating documentation with Doxygen"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

else()
    message(WARNING "Doxygen not found. Documentation target will be skipped.")
endif()

# Include the generated directory
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Add subdirectories for the libraries and tests
add_subdirectory(src/client)
add_subdirectory(src/server)
#add_subdirectory(inc/schema)
add_subdirectory(test)

# Add the executable target and link it with the libraries
add_executable(clientapp ${CMAKE_SOURCE_DIR}/src/client/main.c)
target_compile_features(clientapp PRIVATE c_std_99)

add_executable(serverapp ${CMAKE_SOURCE_DIR}/src/server/main.c)
target_compile_features(serverapp PRIVATE c_std_99)

# Add dependency on the FlatBuffers code generation target
#add_dependencies(clientapp generate_flatbuffers_code)
#add_dependencies(serverapp generate_flatbuffers_code)

# Link libraries to the executable
target_link_libraries(clientapp client)
target_link_libraries(serverapp server)

# Install the executables
install(TARGETS clientapp DESTINATION bin)
install(TARGETS serverapp DESTINATION bin)

# Optionally install other files, like documentation or configuration files
install(DIRECTORY src/server/jobfiles/ DESTINATION bin/jobfiles
        FILES_MATCHING PATTERN "*.txt" PATTERN "*.job")
install(FILES "${CMAKE_SOURCE_DIR}/README.md" DESTINATION doc)
install(FILES "${CMAKE_SOURCE_DIR}/CHANGELOG.md" DESTINATION doc)
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION doc)

# Set the install prefix (this is where files will be installed by default)
#set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

# Set CPack properties for packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "your_email@example.com")
set(CPACK_PACKAGE_DESCRIPTION "This is DemoProject, a sample CMake-based project.")
set(CPACK_PACKAGE_VENDOR "MyCompany")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")  # License file
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_GENERATOR "TGZ")  # Package formats (tar.gz, zip, deb)

# Enable CPack
include(CPack)

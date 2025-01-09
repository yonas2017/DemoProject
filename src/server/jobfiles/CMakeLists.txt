cmake_minimum_required(VERSION 3.31)

# Add executable for generating job files
add_executable(genjob genjob.c)

target_compile_features(client PRIVATE c_std_99)

# Set the directory where the executable will be placed
set_target_properties(genjob PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# List of job files
set(JOB_FILES
    "${CMAKE_CURRENT_LIST_DIR}/a_dolls_house.txt"
    "${CMAKE_CURRENT_LIST_DIR}/alice.txt"
    "${CMAKE_CURRENT_LIST_DIR}/huckleberry.txt"
    "${CMAKE_CURRENT_LIST_DIR}/metamorphosis.txt"
    "${CMAKE_CURRENT_LIST_DIR}/pride_and_prejudice.txt"
)

# Define the output jobs directory path
set(JOB_FILE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/jobfiles")

# Create the directory if it doesn't exist
file(MAKE_DIRECTORY ${JOB_FILE_OUTPUT_DIR})

# Loop through the list of files and add custom commands to execute
set(JOB_TYPES "short" "long")
foreach(JOB_FILE ${JOB_FILES})
    get_filename_component(FILE_NAME ${JOB_FILE} NAME_WE)  # Get just the file name
    foreach(JOB_TYPE ${JOB_TYPES})
        add_custom_command(
            TARGET genjob POST_BUILD
            COMMAND $<TARGET_FILE:genjob> "${JOB_FILE}" "${JOB_FILE_OUTPUT_DIR}/${FILE_NAME}_${JOB_TYPE}.job" "${JOB_TYPE}"
            COMMENT "Running job for ${FILE_NAME}.txt"
        )
    endforeach()
endforeach()

# Create a custom target to trigger the executable execution
add_custom_target(run_jobs ALL DEPENDS genjob)

install(DIRECTORY ${JOB_FILE_OUTPUT_DIR} DESTINATION bin PATTERN "*.job")

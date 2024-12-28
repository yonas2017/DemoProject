#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <flatbuffers/flatbuffers.h>
#include "server_message_generated.h"  // Include the generated header

TEST(ServerSchemaTest, TestServerSchema) {
    // Create a FlatBufferBuilder instance to build the message
    flatbuffers::FlatBufferBuilder builder;

    // Define the job information, job text, and length
    uint8_t job_info = 42;                // Job info as an unsigned byte
    uint32_t job_text_length = 11;        // Length of job text
    std::vector<int8_t> job_tekst = {    // Job text as an array of bytes
        'J', 'o', 'b', ' ', 't', 'e', 'x', 't', ' ', 'd', 'a'
    };

    // Create a flatbuffer table with the above data
    auto job_text_vector = builder.CreateVector(job_tekst);  // Create the vector of bytes for job_tekst
    auto server_message = Createserver_message_t(
        builder,            // FlatBufferBuilder reference
        job_info,           // Job info
        job_text_length,    // Job text length
        job_text_vector     // Job text
    );

    builder.Finish(server_message);  // Finish the buffer with the table

    // Get the pointer to the data inside the FlatBuffer
    uint8_t* buf = builder.GetBufferPointer();

    // Access the deserialized object
    const server_message_t* msg = Getserver_message_t(buf);

    // Print the deserialized data
    std::cout << "Job Info: " << static_cast<int>(msg->job_info()) << "\n";
    std::cout << "Job Text Length: " << msg->job_text_length() << "\n";
    std::cout << "Job Text array size: " << msg->job_tekst()->size() << "\n";

    std::cout << "Job Text: ";
    for (size_t i = 0; i < msg->job_tekst()->size(); ++i) {
        std::cout << static_cast<char>(msg->job_tekst()->Get(i)) << std::flush;
    }
    std::cout << "\n";
    
    ASSERT_EQ(msg->job_info(), job_info);
    ASSERT_EQ(msg->job_text_length(), job_text_length);
    ASSERT_EQ(msg->job_tekst()->size(), job_text_length);
}

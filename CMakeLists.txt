cmake_minimum_required(VERSION 3.12)
project(blemb-proto LANGUAGES C)

# Create the library target
add_library(blemb-proto STATIC
    src/binary.c
    src/buffer.c
    src/crc8.c
    src/protoh.c
    src/protow.c
)

# Include public headers
target_include_directories(blemb-proto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

# Optionally define an install target
install(TARGETS blemb-proto
        EXPORT blemb-proto-targets
        ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Optional: Add examples
option(BLEMB_PROTO_BUILD_EXAMPLES "Build example programs" ON)
if (BLEMB_PROTO_BUILD_EXAMPLES)
    add_executable(sender_example examples/sender.c)
    target_link_libraries(sender_example PRIVATE blemb-proto)

    add_executable(receiver_example examples/receiver.c)
    target_link_libraries(receiver_example PRIVATE blemb-proto)
endif()

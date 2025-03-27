//
//  sender.h
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

#include <stdio.h>
#include <stdlib.h>

#include <blemb/types.h>
#include <blemb/buffer.h>
#include <blemb/protoh.h>

// Called when a complete message has been reassembled
void my_message_handler(blemb_buffer_t message) {
    printf("Received complete message of size: %u\n", message.size);
}

// Optionally validate the message before accepting it
blemb_bool_t my_message_validator(blemb_buffer_t message) {
    // Example: only accept messages smaller than 100 bytes
    return message.size < 100 ? BLEMB_TRUE : BLEMB_FALSE;
}

int main(int argc, char ** argv) {
    blemb_byte_t buffer[128];  // Buffer for reassembly

    blemb_protoh_context_t ctx = {
        .magic = 0xAB,                      // Must match sender's magic
        .buffer_data = buffer,
        .buffer_cur_size = 0,              // MUST be 0 on startup
        .buffer_max_size = sizeof(buffer),
        .validator = my_message_validator, // Can be NULL
        .handler = my_message_handler
    };

    // Simulate receiving a BLE packet
    blemb_byte_t packet_0[] = { 
        0xAB, 0x00, 0x2D, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
    };
    blemb_buffer_t fragment_0 = {
        .data = packet_0,
        .size = sizeof(packet_0)
    };

    if (blemb_protoh_handle(&ctx, fragment_0) == BLEMB_FALSE) {
        // Handle error (e.g., invalid packet or overflow)
        printf("Error handling packet 0\n");

        return EXIT_FAILURE;
    }

    // Simulate receiving a BLE packet
    blemb_byte_t packet_1[] = {
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
        0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24
    };
    blemb_buffer_t fragment_1 = {
        .data = packet_1,
        .size = sizeof(packet_1)
    };

    if (blemb_protoh_handle(&ctx, fragment_1) == BLEMB_FALSE) {
        // Handle error (e.g., invalid packet or overflow)
        printf("Error handling packet 0\n");

        return EXIT_FAILURE;
    }

    // Simulate receiving a BLE packet
    blemb_byte_t packet_2[] = {
        0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x57
    };
    blemb_buffer_t fragment_2 = {
        .data = packet_2,
        .size = sizeof(packet_2)
    };

    if (blemb_protoh_handle(&ctx, fragment_2) == BLEMB_FALSE) {
        // Handle error (e.g., invalid packet or overflow)
        printf("Error handling packet 0\n");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

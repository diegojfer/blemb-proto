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
#include <blemb/protow.h>

// Dummy writer function that would send a packet over BLE
void my_packet_writer(blemb_buffer_t packet) {
    // Send `packet.data` of size `packet.size` over your transport (e.g., BLE GATT)
    // For this example, we just print the size
    printf("Sending packet of size: %u\n", packet.size);
}

int main(int argc, char ** argv) {
    blemb_protow_context_t ctx = {
        .magic = 0xAB,                  // Custom protocol identifier
        .mtu = 20,                      // Example MTU size (e.g., BLE GATT characteristic)
        .writer = my_packet_writer,     // Function used to send each packet
    };

    blemb_byte_t message_data[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
        0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2A, 0x2B, 0x2C
    };

    blemb_buffer_t message = {
        .data = message_data,
        .size = sizeof(message_data),
    };

    if (blemb_protow_write(&ctx, message) == BLEMB_FALSE) {
        // Handle error
        printf("Failed to write message\n");
    }

    return EXIT_SUCCESS;
}

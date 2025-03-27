//
//  protow.c
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

// STDLIB
#include <stddef.h>

// PUBLIC
#include <blemb/types.h>
#include <blemb/buffer.h>
#include <blemb/protow.h>

// PRIVATE
#include <blemb_binary.h>
#include <blemb_buffer.h>
#include <blemb_crc8.h>

blemb_bool_t _blemb_protow_write_message(blemb_byte_t magic, blemb_uint16_t mtu, blemb_buffer_t payload, blemb_protow_writer_f writer) {
    // TODO: Optimize memory usage in _blemb_protow_write_message.
    // Instead of copying the entire message into a new buffer, we can call the writer directly using the payload buffer.
    // The first writer call must copy the first packet (as it includes the magic byte, payload size, and the start of the payload),
    // and the last call must copy the final packet (as it includes the end of the payload and the CRC).
    // Intermediate writer calls can directly use slices of the payload buffer.
    
    // MTU must be at least 1 byte (e.g., MTU 0 is invalid) since we can't split data into packets smaller than 1 byte.
    // We also enforce a maximum MTU of 4096 to prevent potential overflows in the system.
    // Do not remove this limit — the source code does not support larger MTUs and doing so may cause memory overflows.
    if (mtu < 1 || mtu > 4096) return BLEMB_FALSE;
    
    if (writer == NULL) return BLEMB_FALSE;
    
    // Make sure the payload fits in the proto message.
    if (payload.size > BLEMB_UINT16_MAX) return BLEMB_FALSE;
    blemb_uint16_t payload_size = (blemb_uint16_t)payload.size;
    
    // Calculate the message size based on the payload size.
    // This will never overflow because `payload_size` has a maximum value of `UINT16_MAX`,
    // and `4 + UINT16_MAX` easily fits within a 32-bit integer.
    blemb_size_t message_size = 4 + payload_size;
    
    // Compute payload checksum.
    blemb_byte_t checksum = blemb_crc8_compute(payload);
    
    // To support platforms without dynamic memory allocation (e.g., no malloc),
    // we create a buffer that's only used during transmission.
    blemb_byte_t message_data[message_size];
    blemb_buffer_t message_buffer = blemb_buffer_init(message_data, message_size);
    
    // Write packet magic byte to buffer.
    if (blemb_binary_write_byte(message_buffer, 0, magic) != BLEMB_BINARY_RESULT_SUCCESS) return BLEMB_FALSE;
    
    // Write packet payload size to buffer.
    if (blemb_binary_write_uint16(message_buffer, 1, BLEMB_BINARY_ENDIANNESS_BIG, payload_size) != BLEMB_BINARY_RESULT_SUCCESS) return BLEMB_FALSE;
    
    // Write packet payload to buffer.
    if (blemb_binary_write_buffer(message_buffer, 3, payload) != BLEMB_BINARY_RESULT_SUCCESS) return BLEMB_FALSE;
        
    // Write checksum to buffer.
    if (blemb_binary_write_byte(message_buffer, 3 + payload_size, checksum) != BLEMB_BINARY_RESULT_SUCCESS) return BLEMB_FALSE;
    
    // Call writer
    blemb_offset_t offset = 0;
    while (offset < message_buffer.size) {
        blemb_size_t packet_size;
        
        // `offset + mtu` will never overflow.
        // This is guaranteed because `offset` is always less than `message_buffer.size` (enforced by the while loop),
        // and `message_buffer.size` is at most `4 + payload_size`. Since `payload_size` has a maximum of `UINT16_MAX`,
        // the maximum possible value for `offset` is `4 + UINT16_MAX`.
        // Additionally, `mtu` is limited to a maximum of 4096 (enforced by a pre-check).
        // Therefore, the worst-case value for `offset + mtu` is `4 + UINT16_MAX + 4096`,
        // which safely fits within a 32-bit integer.
        if (offset + mtu <= message_buffer.size) {
            packet_size = mtu;
        } else {
            // `message_buffer.size - offset` will not underflow, as this is guaranteed by the while condition.
            packet_size = message_buffer.size - offset;
        }
        
        blemb_buffer_t packet_buffer = blemb_buffer_slice(message_buffer, offset, packet_size);
        if (blemb_buffer_is_empty(packet_buffer) == BLEMB_FALSE) {
            writer(packet_buffer);
        }
        
        // This is guaranteed because `offset` is always less than `message_buffer.size` (enforced by the while loop),
        // and `message_buffer.size` is at most `4 + payload_size`. Since `payload_size` has a maximum of UINT16_MAX,
        // the maximum possible value for `offset` is `4 + UINT16_MAX`.
        // Additionally, `packet_size` is always less than or equal to `mtu`, which is capped at 4096 (enforced by a pre-check).
        // Therefore, the worst-case value for `offset + packet_size` is `4 + UINT16_MAX + 4096`, which safely fits in a 32-bit integer.
        offset = offset + packet_size;
    }
    
    return BLEMB_TRUE;
}

blemb_bool_t blemb_protow_write(blemb_protow_context_t * context, blemb_buffer_t data) {
    if (context == NULL) return BLEMB_FALSE;
    
    return _blemb_protow_write_message(context->magic, context->mtu, data, context->writer);
}

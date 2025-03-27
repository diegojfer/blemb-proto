//
//  buffer.c
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

// STDLIB
#include <stddef.h>

// PUBLIC
#include <blemb/types.h>
#include <blemb/buffer.h>

// PRIVATE
#include <blemb_buffer.h>

static inline blemb_uint32_t _blemb_add_uint32_with_overflow(blemb_uint32_t left, blemb_uint32_t right, blemb_uint32_t * overflow) {
    // Perform 32-bit addition (this may wrap if there's an overflow).
    blemb_uint32_t sum = left + right;
    
    // If an overflow has occurred, the sum will be smaller than either operand.
    if (sum < left || sum < right) {
        if (overflow != NULL) {
            // Calculate how much we exceeded the maximum 32-bit value.
            *overflow = right - (BLEMB_UINT32_MAX - left);
        }
        return BLEMB_UINT32_MAX;
    }
    
    // No overflow occurred, so optionally set overflow to 0.
    if (overflow != NULL) {
        *overflow = 0;
    }
    return sum;
}

blemb_buffer_t blemb_buffer_empty(void) {
    return (struct _blemb_buffer_t) {
        .size = 0,
        .data = NULL,
    };
}

blemb_buffer_t blemb_buffer_init(blemb_byte_t * data, blemb_size_t size) {
    return (struct _blemb_buffer_t) {
        .size = size,
        .data = data,
    };
}

blemb_bool_t blemb_buffer_is_empty(blemb_buffer_t buffer) {
    if (buffer.data == NULL) return BLEMB_TRUE;
    if (buffer.size < 1) return BLEMB_TRUE;
    
    return BLEMB_FALSE;
}

blemb_bool_t blemb_buffer_clear(blemb_buffer_t buffer) {
    if (buffer.data == NULL) return BLEMB_TRUE;
    if (buffer.size < 1) return BLEMB_TRUE;
    
    for (blemb_offset_t i = 0; i < buffer.size; i++) {
        buffer.data[i] = 0x00;
    }
    
    return BLEMB_TRUE;
}

blemb_buffer_t blemb_buffer_slice(blemb_buffer_t buffer, blemb_offset_t offset, blemb_size_t size) {
    if (blemb_buffer_is_empty(buffer) == BLEMB_TRUE) return blemb_buffer_empty();
    
    // Check if requested offset is inside the buffer.
    if (offset >= buffer.size) return blemb_buffer_empty();
    
    // Check if requested size is inside the buffer bounds.
    blemb_uint32_t last_position_overflow = 0;
    blemb_uint32_t last_position = _blemb_add_uint32_with_overflow(offset, size, &last_position_overflow);
    if (last_position_overflow > 0) return blemb_buffer_empty();
    if (last_position > buffer.size) return blemb_buffer_empty();
        
    // TODO: Potential buffer overflow.
    blemb_byte_t * new_data = buffer.data + offset;
    blemb_size_t new_size = size;
    
    return blemb_buffer_init(new_data, new_size);
}

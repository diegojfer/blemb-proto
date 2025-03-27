//
//  protoh.c
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

// STDLIB
#include <stddef.h>

// PUBLIC
#include <blemb/types.h>
#include <blemb/buffer.h>
#include <blemb/protoh.h>

// PRIVATE
#include <blemb_binary.h>
#include <blemb_buffer.h>
#include <blemb_crc8.h>

blemb_size_t _blemb_protoh_validate_message(blemb_buffer_t buffer, blemb_byte_t expected_magic, blemb_protoh_message_validator_f message_validator, blemb_buffer_t * result) {
    // Check if magic is detected!
    blemb_byte_t message_magic = 0;
    if (blemb_binary_read_byte(buffer, 0, &message_magic) != BLEMB_BINARY_RESULT_SUCCESS) {
        *result = blemb_buffer_empty();
        return 0;
    }
    if (message_magic != expected_magic) {
        *result = blemb_buffer_empty();
        return 0;
    }
    
    // Get message size!
    blemb_uint16_t message_size = 0;
    if (blemb_binary_read_uint16(buffer, 1, BLEMB_BINARY_ENDIANNESS_BIG, &message_size) != BLEMB_BINARY_RESULT_SUCCESS) {
        *result = blemb_buffer_empty();
        return 0;
    }
    
    // Get message data!
    blemb_buffer_t message = blemb_buffer_slice(buffer, 3, message_size);
    if (blemb_buffer_is_empty(message) == BLEMB_TRUE && message_size > 0) {
        *result = blemb_buffer_empty();
        return 0;
    }
    
    // Compute CRC8
    blemb_byte_t calculated_checksum = blemb_crc8_compute(message);
    
    blemb_byte_t checksum = 0;
    if (blemb_binary_read_byte(buffer, 3 + message_size, &checksum) != BLEMB_BINARY_RESULT_SUCCESS) {
        *result = blemb_buffer_empty();
        return 0;
    }
    if (checksum != calculated_checksum) {
        *result = blemb_buffer_empty();
        return 0;
    }
    
    if (message_validator != NULL && message_validator(message) == BLEMB_FALSE) {
        *result = blemb_buffer_empty();
        return 0;
    }
    
    *result = message;
    return 1 + 2 + message_size + 1;
}

blemb_size_t _blemb_protoh_try_to_find_next_message(blemb_buffer_t buffer, blemb_byte_t expected_magic, blemb_protoh_message_validator_f message_validator, blemb_buffer_t * result) {
    for (blemb_offset_t offset = 0; offset < buffer.size; offset++) {
        // No need to check for overflows: the for loop ensures `offset` is never greater
        // than the buffer size. Even if it were, `slice` would return an empty buffer.
        // Similarly, `buffer.size - offset` won’t underflow, since `offset` is always lower
        // than buffer size.
        blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, buffer.size - offset);
        
        // Using the new buffer, attempt to parse a valid message.
        blemb_buffer_t message = blemb_buffer_empty();
        blemb_size_t bytes_to_be_processed = _blemb_protoh_validate_message(buf, expected_magic, message_validator, &message);
        
        // If the system is ready to process bytes, a valid message has been detected.
        // Stop searching for a message and return the number of bytes the caller should skip to process it.
        if (bytes_to_be_processed > 0) {
            // No need to check for overflows: `offset + bytes_to_be_processed` won’t overflow,
            // because `bytes_to_be_processed` is always less than the remaining buffer size,
            // in that case, `buffer.size - offset`.
            *result = message;
            return offset + bytes_to_be_processed;
        }
    }
    
    *result = blemb_buffer_empty();
    return 0;
}

blemb_bool_t _blemb_protoh_process_next_message(blemb_protoh_context_t * context) {
    if (context == NULL) return BLEMB_FALSE;
    
    // Create a temporary buffer from the current `protoh` context state.
    blemb_buffer_t buffer = blemb_buffer_init(context->buffer_data, context->buffer_cur_size);
    
    // Using the current `protoh` context state, attempt to parse a valid message.
    blemb_buffer_t message = blemb_buffer_empty();
    blemb_size_t bytes_to_be_processed = _blemb_protoh_try_to_find_next_message(buffer, context->magic, context->validator, &message);
    
    // If the system is ready to process bytes, a valid message has been detected.
    // Notify the caller of the new message, process it, and discard the corresponding
    // bytes from the `protoh` context.
    if (bytes_to_be_processed) {
        // Notify the user.
        if (context->handler != NULL) {
            context->handler(message);
        }
        
        // Move unprocessed data before discarding bytes to free up space.
        for (blemb_offset_t offset = bytes_to_be_processed; offset < context->buffer_cur_size; offset++) {
            context->buffer_data[offset - bytes_to_be_processed] = context->buffer_data[offset];
        }
        // Remove processed bytes to free up buffer space.
        context->buffer_cur_size = context->buffer_cur_size - bytes_to_be_processed;
        
        return BLEMB_TRUE;
    }
    
    return BLEMB_FALSE;
}

void _blemb_protoh_skip_current_candidate(blemb_protoh_context_t * context) {
    if (context == NULL) return;
    
    // If there is no data on the buffer, there is no
    // candidate to be skipped.
    if (context->buffer_cur_size == 0) {
        return;
    }
    
    // If there is one byte only on the buffer, no byte
    // reallocation is needed. Only buffer cleaning.
    if (context->buffer_cur_size == 1) {
        context->buffer_cur_size = 0;
        return;
    }
    
    // If there is more than 2 bytes on the buffer, find
    // next candidate index.
    blemb_offset_t next_candidate_offset = 0;
    for (blemb_offset_t i = 1; i < context->buffer_cur_size; i++) {
        if (context->buffer_data[i] == context->magic) {
            next_candidate_offset = i;
            break;
        }
    }
    
    // If there is no next candidate, no byte reallocation,
    // is needed. Only buffer cleaning.
    if (next_candidate_offset == 0) {
        context->buffer_cur_size = 0;
        return;
    }
    
    // If a candidate is found, we need to reallocate bytes
    // and update buffer_size_current.
    uint32_t new_buffer_size = context->buffer_cur_size - next_candidate_offset;
    
    for (uint32_t index = 0; index < new_buffer_size; index++) {
        context->buffer_data[index] = context->buffer_data[next_candidate_offset + index];
    }
    context->buffer_cur_size = new_buffer_size;
}

blemb_bool_t blemb_protoh_handle(blemb_protoh_context_t * context, blemb_buffer_t data) {
    if (context == NULL) return BLEMB_FALSE;
    
    // To free up space before adding more data, attempt to deliver
    // all pending messages.
    while (_blemb_protoh_process_next_message(context) == BLEMB_TRUE) { }
        
    // If the input data exceeds the maximum processable size, return
    // an error indicating it can't be processed.
    if (data.size > context->buffer_max_size) {
        return BLEMB_FALSE;
    }
    
    // If there's no space to store the received data, skip the
    // unprocessed data.
    blemb_size_t available_space = context->buffer_max_size - context->buffer_cur_size;
    if (data.size > available_space) {
        _blemb_protoh_skip_current_candidate(context);
        
        return blemb_protoh_handle(context, data);
    }
    
    // Write the received data to the buffer.
    for (blemb_offset_t offset = 0; offset < data.size; offset++) {
        context->buffer_data[context->buffer_cur_size + offset] = data.data[offset];
    }
    context->buffer_cur_size += data.size;
    
    // After adding the received data, attempt to deliver all pending messages.
    while (_blemb_protoh_process_next_message(context) == BLEMB_TRUE) { }
    
    return BLEMB_TRUE;
}

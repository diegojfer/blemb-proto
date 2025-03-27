//
//  binary.c
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
#include <blemb_binary.h>

// -----
// UTILS
// -----
blemb_bool_t _blemb_binary_should_rotate(blemb_binary_endianness_t endianness) {
    volatile uint32_t i = 0x01234567;
    
    // 0 for big endian, 1 for little endian
    blemb_bool_t system_is_little_endian = (*((uint8_t*)(&i))) == 0x67;
    
    if (endianness == BLEMB_BINARY_ENDIANNESS_LITTLE) {
        return system_is_little_endian ? BLEMB_FALSE : BLEMB_TRUE;
    }
    if (endianness == BLEMB_BINARY_ENDIANNESS_BIG) {
        return system_is_little_endian ? BLEMB_TRUE : BLEMB_FALSE;
    }
    
    return BLEMB_FALSE;
}

// ----
// BYTE
// ----
blemb_byte_t _blemb_binary_read_unsafe_byte(blemb_byte_t * pointer) {
    return *pointer;
}
void _blemb_binary_write_unsafe_byte(blemb_byte_t * pointer, blemb_byte_t value) {
    *pointer = value;
}

blemb_binary_result_t blemb_binary_read_byte(blemb_buffer_t buffer, blemb_offset_t offset, blemb_byte_t * result) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 1);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        *result = 0;
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    *result = _blemb_binary_read_unsafe_byte(buf.data);
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_byte(blemb_buffer_t buffer, blemb_offset_t offset, blemb_byte_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 1);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    _blemb_binary_write_unsafe_byte(buf.data, value);
    
    return BLEMB_BINARY_RESULT_SUCCESS;
}

// ----
// INT8
// ----
blemb_int8_t _blemb_binary_read_unsafe_int8(blemb_byte_t * pointer) {
    return *((blemb_int8_t *)pointer);
}
void _blemb_binary_write_unsafe_int8(blemb_byte_t * pointer, blemb_int8_t value) {
    *((blemb_int8_t *)pointer) = value;
}

blemb_binary_result_t blemb_binary_read_int8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_int8_t * result) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 1);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        *result = 0;
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    *result = _blemb_binary_read_unsafe_int8(buf.data);
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_int8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_int8_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 1);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    _blemb_binary_write_unsafe_int8(buf.data, value);
    
    return BLEMB_BINARY_RESULT_SUCCESS;
}

// -----
// UINT8
// -----
blemb_uint8_t _blemb_binary_read_unsafe_uint8(blemb_byte_t * pointer) {
    return *((blemb_uint8_t *)pointer);
}
void _blemb_binary_write_unsafe_uint8(blemb_byte_t * pointer, blemb_uint8_t value) {
    *((blemb_uint8_t *)pointer) = value;
}

blemb_binary_result_t blemb_binary_read_uint8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_uint8_t * result) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 1);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        *result = 0;
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    *result = _blemb_binary_read_unsafe_uint8(buf.data);
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_uint8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_uint8_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 1);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    _blemb_binary_write_unsafe_uint8(buf.data, value);
    
    return BLEMB_BINARY_RESULT_SUCCESS;
}

// -----
// INT16
// -----
blemb_int16_t _blemb_binary_read_unsafe_int16(blemb_byte_t * pointer, blemb_binary_endianness_t endianness) {
    blemb_byte_t integer[2];
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        integer[0] = pointer[1];
        integer[1] = pointer[0];
    } else {
        integer[0] = pointer[0];
        integer[1] = pointer[1];
    }
    
    return *((blemb_int16_t *)integer);
}
void _blemb_binary_write_unsafe_int16(blemb_byte_t * pointer, blemb_int16_t value, blemb_binary_endianness_t endianness) {
    blemb_byte_t * integer = (blemb_byte_t *)&value;
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        pointer[0] = integer[1];
        pointer[1] = integer[0];
    } else {
        pointer[0] = integer[0];
        pointer[1] = integer[1];
    }
}

blemb_binary_result_t blemb_binary_read_int16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int16_t * result) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 2);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        *result = 0;
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    *result = _blemb_binary_read_unsafe_int16(buf.data, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_int16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int16_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 2);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    _blemb_binary_write_unsafe_int16(buf.data, value, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}

// ------
// UINT16
// ------
blemb_uint16_t _blemb_binary_read_unsafe_uint16(blemb_byte_t * pointer, blemb_binary_endianness_t endianness) {
    blemb_byte_t integer[2];
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        integer[0] = pointer[1];
        integer[1] = pointer[0];
    } else {
        integer[0] = pointer[0];
        integer[1] = pointer[1];
    }
    
    return *((blemb_uint16_t *)integer);
}
void _blemb_binary_write_unsafe_uint16(blemb_byte_t * pointer, blemb_uint16_t value, blemb_binary_endianness_t endianness) {
    blemb_byte_t * integer = (blemb_byte_t *)&value;
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        pointer[0] = integer[1];
        pointer[1] = integer[0];
    } else {
        pointer[0] = integer[0];
        pointer[1] = integer[1];
    }
}

blemb_binary_result_t blemb_binary_read_uint16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint16_t * result) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 2);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        *result = 0;
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    *result = _blemb_binary_read_unsafe_uint16(buf.data, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_uint16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint16_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 2);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    _blemb_binary_write_unsafe_uint16(buf.data, value, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}

// -----
// INT32
// -----
blemb_int32_t _blemb_binary_read_unsafe_int32(blemb_byte_t * pointer, blemb_binary_endianness_t endianness) {
    blemb_byte_t integer[4];
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        integer[0] = pointer[3];
        integer[1] = pointer[2];
        integer[2] = pointer[1];
        integer[3] = pointer[0];
    } else {
        integer[0] = pointer[0];
        integer[1] = pointer[1];
        integer[2] = pointer[2];
        integer[3] = pointer[3];
    }
    
    return *((blemb_int32_t *)integer);
}
void _blemb_binary_write_unsafe_int32(blemb_byte_t * pointer, blemb_int32_t value, blemb_binary_endianness_t endianness) {
    blemb_byte_t * integer = (blemb_byte_t *)&value;
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        pointer[0] = integer[3];
        pointer[1] = integer[2];
        pointer[2] = integer[1];
        pointer[3] = integer[0];
    } else {
        pointer[0] = integer[0];
        pointer[1] = integer[1];
        pointer[2] = integer[2];
        pointer[3] = integer[3];
    }
}

blemb_binary_result_t blemb_binary_read_int32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int32_t * result) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 4);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        *result = 0;
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    *result = _blemb_binary_read_unsafe_int32(buf.data, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_int32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int32_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 4);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    _blemb_binary_write_unsafe_int32(buf.data, value, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}

// ------
// UINT32
// ------
blemb_uint32_t _blemb_binary_read_unsafe_uint32(blemb_byte_t * pointer, blemb_binary_endianness_t endianness) {
    blemb_byte_t integer[4];
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        integer[0] = pointer[3];
        integer[1] = pointer[2];
        integer[2] = pointer[1];
        integer[3] = pointer[0];
    } else {
        integer[0] = pointer[0];
        integer[1] = pointer[1];
        integer[2] = pointer[2];
        integer[3] = pointer[3];
    }
    
    return *((blemb_uint32_t *)integer);
}
void _blemb_binary_write_unsafe_uint32(blemb_byte_t * pointer, blemb_uint32_t value, blemb_binary_endianness_t endianness) {
    blemb_byte_t * integer = (blemb_byte_t *)&value;
    
    if (_blemb_binary_should_rotate(endianness) == BLEMB_TRUE) {
        pointer[0] = integer[3];
        pointer[1] = integer[2];
        pointer[2] = integer[1];
        pointer[3] = integer[0];
    } else {
        pointer[0] = integer[0];
        pointer[1] = integer[1];
        pointer[2] = integer[2];
        pointer[3] = integer[3];
    }
}

blemb_binary_result_t blemb_binary_read_uint32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint32_t * result) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 4);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        *result = 0;
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    *result = _blemb_binary_read_unsafe_uint32(buf.data, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_uint32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint32_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, 4);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    _blemb_binary_write_unsafe_uint32(buf.data, value, endianness);
    return BLEMB_BINARY_RESULT_SUCCESS;
}

// ------
// BUFFER
// ------
blemb_binary_result_t blemb_binary_read_buffer(blemb_buffer_t buffer, blemb_offset_t offset, blemb_buffer_t * result) {
    if (result == NULL) return BLEMB_BINARY_RESULT_SUCCESS;
    if (blemb_buffer_is_empty(*result) == BLEMB_TRUE) return BLEMB_BINARY_RESULT_SUCCESS;
    
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, result->size);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    for (blemb_offset_t i = 0; i < result->size; i++) {
        result->data[i] = buf.data[i];
    }
    
    return BLEMB_BINARY_RESULT_SUCCESS;
}
blemb_binary_result_t blemb_binary_write_buffer(blemb_buffer_t buffer, blemb_offset_t offset, blemb_buffer_t value) {
    blemb_buffer_t buf = blemb_buffer_slice(buffer, offset, value.size);
    if (blemb_buffer_is_empty(buf) == BLEMB_TRUE) {
        return BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES;
    }
    
    for (blemb_offset_t i = 0; i < buf.size; i++) {
        buf.data[i] = value.data[i];
    }
    
    return BLEMB_BINARY_RESULT_SUCCESS;
}

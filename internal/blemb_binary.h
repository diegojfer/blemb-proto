//
//  blemb_binary.h
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

#ifndef BLEMB_PRIVATE_BINARY_H
#define BLEMB_PRIVATE_BINARY_H

#include <blemb/types.h>
#include <blemb/buffer.h>

typedef blemb_uint8_t blemb_binary_endianness_t;
#define BLEMB_BINARY_ENDIANNESS_LITTLE 0
#define BLEMB_BINARY_ENDIANNESS_BIG 1

typedef blemb_uint8_t blemb_binary_result_t;
#define BLEMB_BINARY_RESULT_SUCCESS 0
#define BLEMB_BINARY_RESULT_FAILED_NOT_ENOUGH_BYTES 1

// BYTE
extern blemb_binary_result_t blemb_binary_read_byte(blemb_buffer_t buffer, blemb_offset_t offset, blemb_byte_t * result);
extern blemb_binary_result_t blemb_binary_write_byte(blemb_buffer_t buffer, blemb_offset_t offset, blemb_byte_t value);

// INT8
extern blemb_binary_result_t blemb_binary_read_int8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_int8_t * result);
extern blemb_binary_result_t blemb_binary_write_int8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_int8_t value);

// UINT8
extern blemb_binary_result_t blemb_binary_read_uint8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_uint8_t * result);
extern blemb_binary_result_t blemb_binary_write_uint8(blemb_buffer_t buffer, blemb_offset_t offset, blemb_uint8_t value);

// INT16
extern blemb_binary_result_t blemb_binary_read_int16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int16_t * result);
extern blemb_binary_result_t blemb_binary_write_int16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int16_t value);

// UINT16
extern blemb_binary_result_t blemb_binary_read_uint16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint16_t * result);
extern blemb_binary_result_t blemb_binary_write_uint16(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint16_t value);

// INT32
extern blemb_binary_result_t blemb_binary_read_int32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int32_t * result);
extern blemb_binary_result_t blemb_binary_write_int32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_int32_t value);

// UINT32
extern blemb_binary_result_t blemb_binary_read_uint32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint32_t * result);
extern blemb_binary_result_t blemb_binary_write_uint32(blemb_buffer_t buffer, blemb_offset_t offset, blemb_binary_endianness_t endianness, blemb_uint32_t value);

// BUFFER
extern blemb_binary_result_t blemb_binary_read_buffer(blemb_buffer_t buffer, blemb_offset_t offset, blemb_buffer_t * result);
extern blemb_binary_result_t blemb_binary_write_buffer(blemb_buffer_t buffer, blemb_offset_t offset, blemb_buffer_t value);

#endif

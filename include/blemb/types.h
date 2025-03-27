//
//  blemb/types.h
//  BLEMB
//
//  Created by Diego Fernandez on 25/3/25.
//

#ifndef BLEMB_TYPES_H
#define BLEMB_TYPES_H

#include <stdint.h>

typedef uint8_t blemb_bool_t;
#define BLEMB_TRUE 1
#define BLEMB_FALSE 0

typedef uint8_t blemb_byte_t;
#define BLEMB_BYTE_MAX UINT8_MAX

typedef int8_t blemb_int8_t;
typedef uint8_t blemb_uint8_t;
#define BLEMB_INT8_MAX INT8_MAX
#define BLEMB_UINT8_MAX UINT8_MAX

typedef int16_t blemb_int16_t;
typedef uint16_t blemb_uint16_t;
#define BLEMB_INT16_MAX INT16_MAX
#define BLEMB_UINT16_MAX UINT16_MAX

typedef int32_t blemb_int32_t;
typedef uint32_t blemb_uint32_t;
#define BLEMB_INT32_MAX INT32_MAX
#define BLEMB_UINT32_MAX UINT32_MAX

typedef uint32_t blemb_size_t;
#define BLEMB_SIZE_MAX UINT32_MAX

typedef blemb_size_t blemb_offset_t;
#define BLEMB_OFFSET_MAX BLEMB_SIZE_MAX

#endif

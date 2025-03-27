//
//  blemb/buffer.h
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

#ifndef BLEMB_BUFFER_H
#define BLEMB_BUFFER_H

#include <blemb/types.h>

typedef struct _blemb_buffer_t {
    blemb_size_t size;
    blemb_byte_t * data;
} blemb_buffer_t;

#endif

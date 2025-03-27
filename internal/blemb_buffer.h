//
//  blemb_buffer.h
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

#ifndef BLEMB_PRIVATE_BUFFER_H
#define BLEMB_PRIVATE_BUFFER_H

#include <blemb/types.h>
#include <blemb/buffer.h>

extern blemb_buffer_t blemb_buffer_empty(void);
extern blemb_buffer_t blemb_buffer_init(blemb_byte_t * data, blemb_size_t size);

extern blemb_bool_t blemb_buffer_is_empty(blemb_buffer_t buffer);

extern blemb_bool_t blemb_buffer_clear(blemb_buffer_t buffer);
extern blemb_buffer_t blemb_buffer_slice(blemb_buffer_t buffer, blemb_offset_t offset, blemb_size_t size);

#endif

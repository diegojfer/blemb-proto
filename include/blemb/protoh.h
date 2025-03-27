//
//  blemb_protoh.h
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

#ifndef BLEMB_PROTOH_H
#define BLEMB_PROTOH_H

#include <blemb/types.h>
#include <blemb/buffer.h>

typedef blemb_bool_t (*blemb_protoh_message_validator_f)(blemb_buffer_t);
typedef void (*blemb_protoh_message_handler_f)(blemb_buffer_t);

typedef struct _blemb_protoh_context_t {
    blemb_byte_t magic;
    
    blemb_uint8_t * buffer_data;
    blemb_uint32_t buffer_cur_size;
    blemb_uint32_t buffer_max_size;
    
    blemb_protoh_message_validator_f validator;
    blemb_protoh_message_handler_f handler;
} blemb_protoh_context_t;

extern blemb_bool_t blemb_protoh_handle(blemb_protoh_context_t * context, blemb_buffer_t data);

#endif

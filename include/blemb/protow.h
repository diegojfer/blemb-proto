//
//  blemb/protow.h
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

#ifndef BLEMB_PROTOW_H
#define BLEMB_PROTOW_H

#include <blemb/types.h>
#include <blemb/buffer.h>

typedef void (*blemb_protow_writer_f)(blemb_buffer_t);

typedef struct _blemb_protow_context_t {
    blemb_byte_t magic;
    blemb_uint16_t mtu;
    
    blemb_protow_writer_f writer;
} blemb_protow_context_t;

extern blemb_bool_t blemb_protow_write(blemb_protow_context_t * context, blemb_buffer_t data);

#endif

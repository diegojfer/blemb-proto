//
//  crc8.c
//  BLEMB
//
//  Created by Diego Fernandez on 26/3/25.
//

// STDLIB

// PUBLIC
#include <blemb/types.h>
#include <blemb/buffer.h>

// PRIVATE
#include <blemb_crc8.h>

blemb_byte_t _blemb_reverse_bits(blemb_byte_t byte) {
    blemb_byte_t result = 0;
    for (blemb_uint32_t i = 0; i < 8; i++) {
        result <<= 1;             // Shift result left
        result |= (byte & 1);     // Copy least significant bit of byte
        byte >>= 1;               // Shift byte right
    }
    return result;
}

blemb_byte_t blemb_crc8_compute(blemb_buffer_t buffer) {
    // Configured to use Bluetooth CRC8.
    blemb_uint8_t initial_crc = 0x00;
    blemb_uint8_t polynom = 0xA7;
    blemb_uint8_t xor = 0x00;
    blemb_bool_t ref_in = BLEMB_FALSE;
    blemb_bool_t ref_out = BLEMB_FALSE;
    
    blemb_byte_t crc = initial_crc;
    
    for (blemb_offset_t i = 0; i < buffer.size; i++) {
        blemb_byte_t byte = buffer.data[i];
        
        if (ref_in == BLEMB_TRUE) {
            crc ^= _blemb_reverse_bits(byte);
        } else {
            crc ^= byte;
        }
        
        for (blemb_offset_t j = 0; j < 8; j++) {
            blemb_byte_t check = crc & 0x80;
            crc <<= 1;
            
            if (check != 0) {
                crc ^= polynom;
            }
        }
    }
        
    if (ref_out == BLEMB_TRUE) {
        return _blemb_reverse_bits(crc) ^ xor;
    } else {
        return crc ^ xor;
    }
}

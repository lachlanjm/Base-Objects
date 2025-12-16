#ifndef SHA_2_BASE_H
#define SHA_2_BASE_H

#include "../Dynamic Array/dyn_array.h"

#include <stdint.h>

#define ROTATE_R(x,n,l) (((x) >> (n)) | ((x) << ((l)-(n))))
#define SHIFT_R(x,n)  ((x) >> (n))

static inline void pad_message_set_min_length(dyn_array* const message, const uint64_t bit_length)
{
    const uint64_t current_bit_length = message->current_size * message->item_size * 8;

    if (bit_length <= current_bit_length) return;

    const uint64_t total_bits_to_add = bit_length - current_bit_length;
    const uint64_t total_bytes_to_add = (total_bits_to_add + 7) / 8; // round up to nearest byte
    const uint64_t total_slots_to_add = total_bytes_to_add * message->item_size;

    for (unsigned int i = 0; i < total_slots_to_add; i++)
    {
        const unsigned char pad_byte = (i==0) ? 0x80 : 0x00; // first byte is 10000000, rest are 00000000
        void* slot = add_slot_dyn_array(message);

        *((unsigned char*)slot) = pad_byte;
        for (int j = 1; j < message->item_size; j++) *((unsigned char*)slot+j) = 0x00;
    }
}

static inline void pad_message_digest_length(dyn_array* const message, const unsigned int block_bit_length, const unsigned int length_field_bit_size)
{
    const uint64_t current_bit_length = message->current_size * message->item_size * 8;
    const uint64_t current_plus_length_field = current_bit_length + length_field_bit_size;

    const uint64_t needed_extra_bits = block_bit_length - (((current_plus_length_field-1) % block_bit_length) + 1);
    const uint64_t new_bit_length = current_bit_length + needed_extra_bits;

    pad_message_set_min_length(message, new_bit_length);
}

static inline void append_message_length(dyn_array* const message, const unsigned int length_field_bit_size, const uint64_t original_message_bit_length)
{
    // SHA-2 appends the original message length as a 64-bit big-endian integer
    for (int i = (length_field_bit_size/8-1); i >= 0; i--)
    {
        unsigned char byte = (original_message_bit_length >> (i * 8)) & 0xFF;
        void* slot = add_slot_dyn_array(message);

        *((unsigned char*)slot) = byte;
        for (int j = 1; j < message->item_size; j++) *((unsigned char*)slot+j) = 0x00;
    }
}



#endif
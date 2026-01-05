#ifndef SHA_256_H
#define SHA_256_H

#include "SHA-2-base.h"
#include "../Dynamic Array/dyn_array.h"

#include <stdint.h>

#define SHA_256_BLOCK_BIT_LENGTH 512
#define SHA_256_LENGTH_FIELD_BIT_SIZE 64
#define SHA_256_WORD_SIZE 32
#define SHA_256_NUM_ROUNDS 64

#define K_256_INITIALIZER { \
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, \
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, \
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, \
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, \
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, \
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, \
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, \
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2, \
}

#define H_256_INITIALIZER { \
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19, \
}

/**
 * @brief Computes the SHA-256 hash of the input message and returns the hexadecimal digest. 
 * @param message Pointer to a dyn_array containing the input message bytes. Must be of type DYN_ARRAY_CHAR_TYPE, DYN_ARRAY_INT_TYPE, DYN_ARRAY_UINT_TYPE, or DYN_ARRAY_UINT_64T_TYPE.
 * @return Pointer to a dyn_array containing the SHA-256 hash as a sequence of 32-bit unsigned integers. Returns NULL on error.
 */
static inline dyn_array* hexdigest_sha256(dyn_array* const message)
{
    if (message->type != DYN_ARRAY_CHAR_TYPE && message->type != DYN_ARRAY_INT_TYPE
        && message->type != DYN_ARRAY_UINT_TYPE && message->type != DYN_ARRAY_UINT_8T_TYPE
        && message->type != DYN_ARRAY_UINT_16T_TYPE && message->type != DYN_ARRAY_UINT_32T_TYPE
        && message->type != DYN_ARRAY_UINT_64T_TYPE
    ) { return NULL; }

    const uint32_t original_bit_length = message->current_size * message->item_size * 8;
    pad_message_digest_length(message, SHA_256_BLOCK_BIT_LENGTH, SHA_256_LENGTH_FIELD_BIT_SIZE); 
    append_message_length(message, SHA_256_LENGTH_FIELD_BIT_SIZE, original_bit_length);

    const uint32_t k_vals[] = K_256_INITIALIZER;
    uint32_t h_vals[] = H_256_INITIALIZER;
    uint32_t w_vals[SHA_256_NUM_ROUNDS];
    const uint64_t total_blocks = (message->current_size * message->item_size * 8) / SHA_256_BLOCK_BIT_LENGTH;
    
    for (uint64_t block_i = 0; block_i < total_blocks; block_i++)
    {
        // Prepare the message schedule
        for (unsigned int t = 0; t < 16; t++)
        {
            w_vals[t] = 0;
            for (unsigned int byte_i = 0; byte_i < (SHA_256_WORD_SIZE/8); byte_i++)
            {
                unsigned char byte = (unsigned char)*get_dyn_array_byte(message, block_i * (SHA_256_BLOCK_BIT_LENGTH/8) + t * (SHA_256_WORD_SIZE/8) + byte_i);
                w_vals[t] = (w_vals[t] << 8) | byte;
            }
        }
        // Extend the first 16 words into the remaining words w[16..79] of the message schedule array
        for (unsigned int t = 16; t < SHA_256_NUM_ROUNDS; t++)
        {
            const uint32_t s0 = ROTATE_R(w_vals[t-15], 7, SHA_256_WORD_SIZE) ^ ROTATE_R(w_vals[t-15], 18, SHA_256_WORD_SIZE) ^ SHIFT_R(w_vals[t-15], 3);
            const uint32_t s1 = ROTATE_R(w_vals[t-2], 17, SHA_256_WORD_SIZE) ^ ROTATE_R(w_vals[t-2], 19, SHA_256_WORD_SIZE) ^ SHIFT_R(w_vals[t-2], 10);
            w_vals[t] = w_vals[t-16] + s0 + w_vals[t-7] + s1;
        }

        uint32_t a = h_vals[0];
        uint32_t b = h_vals[1];
        uint32_t c = h_vals[2];
        uint32_t d = h_vals[3];
        uint32_t e = h_vals[4];
        uint32_t f = h_vals[5];
        uint32_t g = h_vals[6];
        uint32_t h = h_vals[7];

        // Main compression function
        for (unsigned int t = 0; t < SHA_256_NUM_ROUNDS; t++)
        {
            const uint32_t S1 = ROTATE_R(e, 6, SHA_256_WORD_SIZE) ^ ROTATE_R(e, 11, SHA_256_WORD_SIZE) ^ ROTATE_R(e, 25, SHA_256_WORD_SIZE);
            const uint32_t ch = (e & f) ^ ((~e) & g);
            const uint32_t temp1 = h + S1 + ch + k_vals[t] + w_vals[t];
            const uint32_t S0 = ROTATE_R(a, 2, SHA_256_WORD_SIZE) ^ ROTATE_R(a, 13, SHA_256_WORD_SIZE) ^ ROTATE_R(a, 22, SHA_256_WORD_SIZE);
            const uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            const uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h_vals[0] += a;
        h_vals[1] += b;
        h_vals[2] += c;
        h_vals[3] += d;
        h_vals[4] += e;
        h_vals[5] += f;
        h_vals[6] += g;
        h_vals[7] += h;
    }

    dyn_array* hex_digest = new_dyn_array(DYN_ARRAY_UINT_32T_TYPE, DYN_ARRAY_EXPANSION_FIXED);
    for (unsigned int i = 0; i < 8; i++)
    {
        append_item_dyn_array(hex_digest, &h_vals[i]);
    }
    return hex_digest;
}


#endif
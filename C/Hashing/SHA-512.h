#ifndef SHA_512_H
#define SHA_512_H

#include "SHA-2-base.h"
#include "../Dynamic Array/dyn_array.h"

#include <stdint.h>

#define SHA_512_BLOCK_BIT_LENGTH 1024
#define SHA_512_LENGTH_FIELD_BIT_SIZE 128
#define SHA_512_WORD_SIZE 64
#define SHA_512_NUM_ROUNDS 80

#define K_512_INITIALIZER { \
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, \
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, \
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, \
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, \
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, \
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, \
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, \
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70, \
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, \
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, \
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, \
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, \
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, \
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, \
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, \
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, \
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, \
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b, \
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, \
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817, \
}

#define H_512_INITIALIZER { \
    0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, \
    0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179, \
}

/**
 * @brief Computes the SHA-512 hash of the input message and returns the hexadecimal digest. 
 * @param message Pointer to a dyn_array containing the input message bytes. Must be of type DYN_ARRAY_CHAR_TYPE, DYN_ARRAY_INT_TYPE, DYN_ARRAY_UINT_TYPE, or DYN_ARRAY_UINT_64T_TYPE.
 * @return Pointer to a dyn_array containing the SHA-512 hash as a sequence of 64-bit unsigned integers. Returns NULL on error.
 */
static inline dyn_array* hexdigest_sha512(dyn_array* const message)
{
    if (message->type != DYN_ARRAY_CHAR_TYPE || message->type != DYN_ARRAY_INT_TYPE
        || message->type != DYN_ARRAY_UINT_TYPE || message->type != DYN_ARRAY_UINT_64T_TYPE
    ) { return NULL; }

    const uint64_t original_bit_length = message->current_size * message->item_size * 8;
    pad_message_digest_length(message, SHA_512_BLOCK_BIT_LENGTH, SHA_512_LENGTH_FIELD_BIT_SIZE);
    append_message_length(message, SHA_512_LENGTH_FIELD_BIT_SIZE, original_bit_length);

    const uint64_t k_vals[] = K_512_INITIALIZER;
    uint64_t h_vals[] = H_512_INITIALIZER;
    uint64_t w_vals[80];
    const unsigned int total_blocks = (message->current_size * message->item_size * 8) / SHA_512_BLOCK_BIT_LENGTH;
    
    for (uint64_t block_i = 0; block_i < total_blocks; block_i++)
    {
        // Prepare the message schedule
        for (unsigned int t = 0; t < 16; t++)
        {
            w_vals[t] = 0;
            for (unsigned int byte_i = 0; byte_i < (SHA_512_WORD_SIZE/8); byte_i++)
            {
                unsigned char byte = *((unsigned char*)get_dyn_array_byte(message, block_i * (SHA_512_BLOCK_BIT_LENGTH/8) + t * (SHA_512_WORD_SIZE/8) + byte_i));
                w_vals[t] = (w_vals[t] << 8) | byte;
            }
        }
        // Extend the first 16 words into the remaining words w[16..79] of the message schedule array
        for (unsigned int t = 16; t < SHA_512_NUM_ROUNDS; t++)
        {
            const uint64_t s0 = ROTATE_R(w_vals[t-15], 1, SHA_512_WORD_SIZE) ^ ROTATE_R(w_vals[t-15], 8, SHA_512_WORD_SIZE) ^ SHIFT_R(w_vals[t-15], 7);
            const uint64_t s1 = ROTATE_R(w_vals[t-2], 19, SHA_512_WORD_SIZE) ^ ROTATE_R(w_vals[t-2], 61, SHA_512_WORD_SIZE) ^ SHIFT_R(w_vals[t-2], 6);
            w_vals[t] = w_vals[t-16] + s0 + w_vals[t-7] + s1;
        }

        uint64_t a = h_vals[0];
        uint64_t b = h_vals[1];
        uint64_t c = h_vals[2];
        uint64_t d = h_vals[3];
        uint64_t e = h_vals[4];
        uint64_t f = h_vals[5];
        uint64_t g = h_vals[6];
        uint64_t h = h_vals[7];

        // Main compression function
        for (unsigned int t = 0; t < SHA_512_NUM_ROUNDS; t++)
        {
            const uint64_t S1 = ROTATE_R(e, 14, SHA_512_WORD_SIZE) ^ ROTATE_R(e, 18, SHA_512_WORD_SIZE) ^ ROTATE_R(e, 41, SHA_512_WORD_SIZE);
            const uint64_t ch = (e & f) ^ ((~e) & g);
            const uint64_t temp1 = h + S1 + ch + k_vals[t] + w_vals[t];
            const uint64_t S0 = ROTATE_R(a, 28, SHA_512_WORD_SIZE) ^ ROTATE_R(a, 34, SHA_512_WORD_SIZE) ^ ROTATE_R(a, 39, SHA_512_WORD_SIZE);
            const uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
            const uint64_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = a + temp1;
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

    dyn_array* hex_digest = new_dyn_array(DYN_ARRAY_UINT_64T_TYPE, DYN_ARRAY_EXPANSION_FIXED);
    for (unsigned int i = 0; i < 8; i++)
    {
        append_item_dyn_array(hex_digest, (void*)h_vals[i]);
    }
    return hex_digest;
}


#endif
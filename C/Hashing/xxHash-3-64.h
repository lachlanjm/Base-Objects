#ifndef XXHASH_3_64_H
#define XXHASH_3_64_H

#include "xxHash-3-base.h"
#include "..\Dynamic Array\dyn_array.h"

#include <stdint.h>

#include <stdio.h>
static inline uint64_t __XXH3_64_short__(dyn_array* const message) 
{
    const uint64_t byte_length = message->current_size * message->item_size;
    const uint64_t seed = XXH3_SEED_DEFAULT;

    if (byte_length < 0) return 0;
    else if (byte_length == 0)
    {
        const uint64_t secret_word_1 = read_64_LE(XXH3_defaultSecret + 56);
        const uint64_t secret_word_2 = read_64_LE(XXH3_defaultSecret + 64);
        return XXH3_avalanche_XXH64(seed ^ secret_word_1 ^ secret_word_2);
    }
    else if (byte_length <= 3) 
    {
        const uint64_t combined = ((uint64_t)*get_dyn_array_byte(message, byte_length-1))
            | (byte_length << 8)
            | (((uint64_t)*get_dyn_array_byte(message, 0)) << 16)
            | (((uint64_t)*get_dyn_array_byte(message, byte_length >> 1)) << 24);
        const uint64_t secret_word_1 = read_32_LE(XXH3_defaultSecret);
        const uint64_t secret_word_2 = read_32_LE(XXH3_defaultSecret + 4);
        return XXH3_avalanche_XXH64(((uint64_t)(secret_word_1 ^ secret_word_2) + seed) ^ combined);
    }
    else if (byte_length <= 8) 
    {
        const uint32_t input_first = read_32_LE(get_dyn_array_byte(message, 0));
        const uint32_t input_last = read_32_LE(get_dyn_array_byte(message, byte_length-4));
        
        const uint64_t modifiedSeed = seed ^ (((uint64_t)BSWAP32(LOWER_HALF_64(seed))) << 32);

        const uint64_t secret_word_1 = read_64_LE(XXH3_defaultSecret + 8);
        const uint64_t secret_word_2 = read_64_LE(XXH3_defaultSecret + 16);
        const uint64_t combined = ((uint64_t)input_last) | (((uint64_t)input_first) << 32);

        uint64_t value = combined ^ ((secret_word_1 ^ secret_word_2) - modifiedSeed);
        value = value ^ ROT_L64(value, 49) ^ ROT_L64(value, 24);
        value = value * PRIME_MX2;
        value = value ^ ((value >> 35) + byte_length);
        value = value * PRIME_MX2;
        value = value ^ (value >> 28);
        return value;
    }
    else if (byte_length <= 16)
    {
        const uint64_t input_first = read_64_LE(get_dyn_array_byte(message, 0));
        const uint64_t input_last  = read_64_LE(get_dyn_array_byte(message, byte_length-8));
        
        const uint64_t secret_word_1 = read_64_LE(XXH3_defaultSecret + 24);
        const uint64_t secret_word_2 = read_64_LE(XXH3_defaultSecret + 32);
        const uint64_t secret_word_3 = read_64_LE(XXH3_defaultSecret + 40);
        const uint64_t secret_word_4 = read_64_LE(XXH3_defaultSecret + 48);

        const uint64_t low = ((secret_word_1 ^ secret_word_2) + seed) ^ input_first;
        const uint64_t high = ((secret_word_3 ^ secret_word_4) - seed) ^ input_last;
        const uint64_t mul_high = MUL_HI_64_128(low, high);
        const uint64_t mul_low = (low * high);
        uint64_t value = byte_length + BSWAP64(low) + high + (mul_low ^ mul_high);
        return XXH3_avalanche(value);
    }

    return 0;
}

static inline uint64_t __XXH3_64_medium__(dyn_array* const message) 
{
    const uint64_t byte_length = message->current_size * message->item_size;
    const uint64_t seed = XXH3_SEED_DEFAULT;

    if (byte_length < 17) return 0;
    else if (byte_length > 240) return 0;

    uint64_t acc = byte_length * PRIME64_1;

    if (byte_length <= 128) 
    {
        const uint64_t numRounds = ((byte_length - 1) >> 5) + 1;
        for (int i = numRounds - 1; i >= 0; i--) {
            const int offsetStart = i*16;
            const int offsetEnd = byte_length - i*16 - 16;
            acc += XXH3_mix_step_XXH64(
                read_64_LE(get_dyn_array_byte(message, offsetStart)),
                read_64_LE(get_dyn_array_byte(message, offsetStart + 8)),
                read_64_LE(XXH3_defaultSecret + i*32),
                read_64_LE(XXH3_defaultSecret + i*32 + 8),
                seed
            );
            acc += XXH3_mix_step_XXH64(
                read_64_LE(get_dyn_array_byte(message, offsetEnd)),
                read_64_LE(get_dyn_array_byte(message, offsetEnd + 8)),
                read_64_LE(XXH3_defaultSecret + i*32 + 16),
                read_64_LE(XXH3_defaultSecret + i*32 + 24),
                seed
            );
        }
    } 
    else 
    {
        const uint64_t numChunks = byte_length >> 4;
        for (int i = 0; i < 8; i++) {
            acc += XXH3_mix_step_XXH64(
                read_64_LE(get_dyn_array_byte(message, i*16)),
                read_64_LE(get_dyn_array_byte(message, i*16 + 8)),
                read_64_LE(XXH3_defaultSecret + i*16),
                read_64_LE(XXH3_defaultSecret + i*16 + 8),
                seed
            );
        }
        acc = XXH3_avalanche(acc);
        for (int i = 8; i < numChunks; i++) {
            acc += XXH3_mix_step_XXH64(
                read_64_LE(get_dyn_array_byte(message, i*16)),
                read_64_LE(get_dyn_array_byte(message, i*16 + 8)),
                read_64_LE(XXH3_defaultSecret + ((i - 8) * 16) + 3),
                read_64_LE(XXH3_defaultSecret + ((i - 8) * 16) + 11),
                seed
            );
        }
        acc += XXH3_mix_step_XXH64(
            read_64_LE(get_dyn_array_byte(message, byte_length - 16)),
            read_64_LE(get_dyn_array_byte(message, byte_length - 8)),
            read_64_LE(XXH3_defaultSecret + 119),
            read_64_LE(XXH3_defaultSecret + 127),
            seed
        );
    }

    return XXH3_avalanche(acc);
}

static inline uint64_t __XXH3_64_long__(dyn_array* const message) 
{
    return 0; // Placeholder for full implementation
}

/**
 * Calculates the XXH3 64-bit hash of the given dynamic array message.
 * @param message Pointer to the dynamic array containing the message data.
 * @return The 64-bit hash value.
 * @warning Currently does not support messages longer than 240 bytes.
 */
static inline uint64_t digest_XXH3_64(dyn_array* const message) 
{
    if (message->type != DYN_ARRAY_CHAR_TYPE && message->type != DYN_ARRAY_INT_TYPE
        && message->type != DYN_ARRAY_UINT_TYPE && message->type != DYN_ARRAY_UINT_8T_TYPE
        && message->type != DYN_ARRAY_UINT_16T_TYPE && message->type != DYN_ARRAY_UINT_32T_TYPE
        && message->type != DYN_ARRAY_UINT_64T_TYPE
    ) { return 0; }

    const uint64_t byte_length = message->current_size * message->item_size;

    if (byte_length <= 16) 
    {
        return __XXH3_64_short__(message);
    }
    else if (byte_length <= 240) 
    {
        return __XXH3_64_medium__(message);
    } 
    else 
    {
        return __XXH3_64_long__(message);
    }
}


#endif
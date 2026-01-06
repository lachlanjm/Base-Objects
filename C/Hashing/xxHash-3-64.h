#ifndef XXHASH_3_64_H
#define XXHASH_3_64_H

#include "xxHash-3-base.h"
#include "..\Dynamic Array\dyn_array.h"

#include <stdint.h>

/**
 * Computes the XXH3 64-bit hash of the given message. (short messages; up to 16 bytes)
 * @param message Pointer to the message as a dynamic array.
 * @param seed The seed value used for hashing.
 * @param secret The secret key used for hashing.
 * @param secret_length The length of the secret key in bytes. (not needed)
 * @return The 64-bit hash value.
 * @warning This function does not check validity of input parameters (outside of message length). Check official specs for more.
 */
static inline uint64_t __XXH3_64_short__(const dyn_array* const message, const uint64_t seed, const uint8_t* const secret, const uint64_t secret_length) 
{
    const uint64_t byte_length = message->current_size * message->item_size;

    if (byte_length < 0) return 0;
    else if (byte_length == 0)
    {
        const uint64_t secret_word_1 = read_64_LE(secret + 56);
        const uint64_t secret_word_2 = read_64_LE(secret + 64);
        return XXH3_avalanche_XXH64(seed ^ secret_word_1 ^ secret_word_2);
    }
    else if (byte_length <= 3) 
    {
        const uint64_t combined = ((uint64_t)*get_dyn_array_byte(message, byte_length-1))
            | (byte_length << 8)
            | (((uint64_t)*get_dyn_array_byte(message, 0)) << 16)
            | (((uint64_t)*get_dyn_array_byte(message, byte_length >> 1)) << 24);
        const uint64_t secret_word_1 = read_32_LE(secret);
        const uint64_t secret_word_2 = read_32_LE(secret + 4);
        return XXH3_avalanche_XXH64(((uint64_t)(secret_word_1 ^ secret_word_2) + seed) ^ combined);
    }
    else if (byte_length <= 8) 
    {
        const uint32_t input_first = read_32_LE(get_dyn_array_byte(message, 0));
        const uint32_t input_last = read_32_LE(get_dyn_array_byte(message, byte_length-4));
        
        const uint64_t modifiedSeed = seed ^ (((uint64_t)BSWAP32(LOWER_HALF_64(seed))) << 32);

        const uint64_t secret_word_1 = read_64_LE(secret + 8);
        const uint64_t secret_word_2 = read_64_LE(secret + 16);
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
        
        const uint64_t secret_word_1 = read_64_LE(secret + 24);
        const uint64_t secret_word_2 = read_64_LE(secret + 32);
        const uint64_t secret_word_3 = read_64_LE(secret + 40);
        const uint64_t secret_word_4 = read_64_LE(secret + 48);

        const uint64_t low = ((secret_word_1 ^ secret_word_2) + seed) ^ input_first;
        const uint64_t high = ((secret_word_3 ^ secret_word_4) - seed) ^ input_last;
        const uint64_t mul_high = MUL_HI_64_128(low, high);
        const uint64_t mul_low = (low * high);
        uint64_t value = byte_length + BSWAP64(low) + high + (mul_low ^ mul_high);
        return XXH3_avalanche(value);
    }

    return 0;
}

/**
 * Computes the XXH3 64-bit hash of the given message. (medium-length messages; 17 to 240 bytes)
 * @param message Pointer to the message as a dynamic array.
 * @param seed The seed value used for hashing.
 * @param secret The secret key used for hashing.
 * @param secret_length The length of the secret key in bytes. (not needed)
 * @return The 64-bit hash value.
 * @warning This function does not check validity of input parameters (outside of message length). Check official specs for more.
 */
static inline uint64_t __XXH3_64_medium__(const dyn_array* const message, const uint64_t seed, const uint8_t* const secret, const uint64_t secret_length) 
{
    const uint64_t byte_length = message->current_size * message->item_size;

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
                read_64_LE(secret + i*32),
                read_64_LE(secret + i*32 + 8),
                seed
            );
            acc += XXH3_mix_step_XXH64(
                read_64_LE(get_dyn_array_byte(message, offsetEnd)),
                read_64_LE(get_dyn_array_byte(message, offsetEnd + 8)),
                read_64_LE(secret + i*32 + 16),
                read_64_LE(secret + i*32 + 24),
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
                read_64_LE(secret + i*16),
                read_64_LE(secret + i*16 + 8),
                seed
            );
        }
        acc = XXH3_avalanche(acc);
        for (int i = 8; i < numChunks; i++) {
            acc += XXH3_mix_step_XXH64(
                read_64_LE(get_dyn_array_byte(message, i*16)),
                read_64_LE(get_dyn_array_byte(message, i*16 + 8)),
                read_64_LE(secret + ((i - 8) * 16) + 3),
                read_64_LE(secret + ((i - 8) * 16) + 11),
                seed
            );
        }
        acc += XXH3_mix_step_XXH64(
            read_64_LE(get_dyn_array_byte(message, byte_length - 16)),
            read_64_LE(get_dyn_array_byte(message, byte_length - 8)),
            read_64_LE(secret + 119),
            read_64_LE(secret + 127),
            seed
        );
    }

    return XXH3_avalanche(acc);
}

/**
 * Computes the XXH3 64-bit hash of the given message. (long messages; more than 240 bytes)
 * @param message Pointer to the message as a dynamic array.
 * @param seed The seed value used for hashing.
 * @param secret The secret key used for hashing.
 * @param secret_length The length of the secret key in bytes.
 * @return The 64-bit hash value.
 * @warning This function does not check validity of input parameters (outside of message length). Check official specs for more.
 */
static inline uint64_t __XXH3_64_long__(const dyn_array* const message, const uint64_t seed, const uint8_t* const secret, const uint64_t secret_length) 
{
    const uint64_t byte_length = message->current_size * message->item_size;

    // secretLength                                            // default 192; at least 136
    const uint64_t stripes_per_block = (secret_length-64) / 8; // default 16; at least 9
    const uint64_t block_size = 64 * stripes_per_block;        // default 1024; at least 576
    
    uint64_t acc[8] = {
        PRIME32_3, PRIME64_1, PRIME64_2, PRIME64_3,
        PRIME64_4, PRIME32_2, PRIME64_5, PRIME32_1
    };
    
    const uint64_t block_rounds = (byte_length - 1) / block_size;
    for (uint64_t block_i = 0; block_i < block_rounds; block_i++) 
    {
        const uint64_t block_index = block_i * block_size;
        XXH3_round_accumulate(acc, get_dyn_array_byte(message, block_index), stripes_per_block, secret);
        XXH3_round_scramble(acc, secret + secret_length - 64);
    }

    const uint64_t remaining_bytes = byte_length - (block_rounds * block_size);
    const uint64_t remaining_stripes = (remaining_bytes - 1) / 64;

    const uint8_t* const message_offset = get_dyn_array_byte(message, block_rounds * block_size);
    const uint8_t* const message_last_stripe = get_dyn_array_byte(message, byte_length - 64);

    XXH3_last_round(acc, message_offset, message_last_stripe, remaining_stripes, secret, secret_length);
    
    return XXH3_final_merge(acc, byte_length * PRIME64_1, secret + 11);
}

/**
 * Calculates the XXH3 64-bit hash of the given dynamic array message.
 * @param message Pointer to the dynamic array containing the message data.
 * @return The 64-bit hash value.
 */
static inline uint64_t digest_XXH3_64(dyn_array* const message) 
{
    if (message->type != DYN_ARRAY_CHAR_TYPE && message->type != DYN_ARRAY_INT_TYPE
        && message->type != DYN_ARRAY_UINT_TYPE && message->type != DYN_ARRAY_UINT_8T_TYPE
        && message->type != DYN_ARRAY_UINT_16T_TYPE && message->type != DYN_ARRAY_UINT_32T_TYPE
        && message->type != DYN_ARRAY_UINT_64T_TYPE
    ) { return 0; }

    const uint64_t byte_length = message->current_size * message->item_size;

    const uint64_t seed = XXH3_SEED_DEFAULT;
    const uint8_t* const secret = XXH3_defaultSecret;
    const uint64_t secret_length = 192;

    if (byte_length <= 16) 
    {
        return __XXH3_64_short__(message, seed, secret, secret_length);
    }
    else if (byte_length <= 240) 
    {
        return __XXH3_64_medium__(message, seed, secret, secret_length);
    } 
    else 
    {
        return __XXH3_64_long__(message, seed, secret, secret_length);
    }
}

/**
 * Calculates the XXH3 64-bit hash of the given dynamic array message with a specified seed.
 * @param message Pointer to the dynamic array containing the message data.
 * @param seed The seed value used for hashing.
 * @return The 64-bit hash value.
 * @warning Untested and unverified!
 */
static inline uint64_t digest_XXH3_64_with_seed(dyn_array* const message, const uint64_t seed) 
{
    if (message->type != DYN_ARRAY_CHAR_TYPE && message->type != DYN_ARRAY_INT_TYPE
        && message->type != DYN_ARRAY_UINT_TYPE && message->type != DYN_ARRAY_UINT_8T_TYPE
        && message->type != DYN_ARRAY_UINT_16T_TYPE && message->type != DYN_ARRAY_UINT_32T_TYPE
        && message->type != DYN_ARRAY_UINT_64T_TYPE
    ) { return 0; }

    const uint64_t byte_length = message->current_size * message->item_size;
    const uint64_t secret_length = 192;

    if (byte_length <= 16) 
    {
        const uint8_t* const secret = XXH3_defaultSecret;
        return __XXH3_64_short__(message, seed, secret, secret_length);
    }
    else if (byte_length <= 240) 
    {
        const uint8_t* const secret = XXH3_defaultSecret;
        return __XXH3_64_medium__(message, seed, secret, secret_length);
    } 
    else 
    {
        uint8_t secret[192] = {0};
        XXH3_derive_secret(secret, seed);
        return __XXH3_64_long__(message, seed, secret, secret_length);
    }
}

/**
 * Calculates the XXH3 64-bit hash of the given dynamic array message with a specified secret.
 * @param message Pointer to the dynamic array containing the message data.
 * @param secret The secret value used for hashing. (at least 136 bytes in Little-Endian convention)
 * @param secret_length The length of the secret in bytes.
 * @return The 64-bit hash value.
 * @warning Untested and unverified!
 */
static inline uint64_t digest_XXH3_64_with_secret(dyn_array* const message, const uint8_t* const secret, const uint64_t secret_length) 
{
    if (message->type != DYN_ARRAY_CHAR_TYPE && message->type != DYN_ARRAY_INT_TYPE
        && message->type != DYN_ARRAY_UINT_TYPE && message->type != DYN_ARRAY_UINT_8T_TYPE
        && message->type != DYN_ARRAY_UINT_16T_TYPE && message->type != DYN_ARRAY_UINT_32T_TYPE
        && message->type != DYN_ARRAY_UINT_64T_TYPE
    ) { return 0; }

    if (secret_length < 136) return 0; // Minimum secret length for XXH3-64

    const uint64_t byte_length = message->current_size * message->item_size;
    const uint64_t seed = XXH3_SEED_DEFAULT;

    if (byte_length <= 16) 
    {
        return __XXH3_64_short__(message, seed, secret, secret_length);
    }
    else if (byte_length <= 240) 
    {
        return __XXH3_64_medium__(message, seed, secret, secret_length);
    } 
    else 
    {
        return __XXH3_64_long__(message, seed, secret, secret_length);
    }
}


#endif
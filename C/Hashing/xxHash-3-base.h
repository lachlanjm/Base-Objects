#ifndef XXHASH_3_BASE_H
#define XXHASH_3_BASE_H

#include "../Dynamic Array/dyn_array.h"

#include <stdint.h>

#define XXH3_SEED_DEFAULT 0ULL

#define BSWAP32(x) ( ((x) >> 24) | (((x) >> 8) & 0x0000FF00U) | (((x) << 8) & 0x00FF0000U) | ((x) << 24) )
#define BSWAP64(x) ( ((x) >> 56) | \
                     (((x) >> 40) & 0x000000000000FF00ULL) | (((x) >> 24) & 0x0000000000FF0000ULL) | \
                     (((x) >> 8)  & 0x00000000FF000000ULL) | (((x) << 8)  & 0x000000FF00000000ULL) | \
                     (((x) << 24) & 0x0000FF0000000000ULL) | (((x) << 40) & 0x00FF000000000000ULL) | \
                     ((x) << 56) )
#define LOWER_HALF_32(x) ((uint32_t)(x) & 0x0000FFFFU)
#define LOWER_HALF_64(x) ((uint64_t)(x) & 0x00000000FFFFFFFFULL)

#define ROT_L32(x, r) ( ((x) << (r)) | ((x) >> (32 - (r))) )
#define ROT_R32(x, r) ( ((x) >> (r)) | ((x) << (32 - (r))) )
#define ROT_L64(x, r) ( ((x) << (r)) | ((x) >> (64 - (r))) )
#define ROT_R64(x, r) ( ((x) >> (r)) | ((x) << (64 - (r))) )

#define BIT_FLAG_32 (0xFFFFFFFFU)
#define MUL_HI_64_128(a, b) (((a) >> 32) * ((b) >> 32) \
                    + ((((a) >> 32) * ((b)&BIT_FLAG_32))>>32) \
                    + ((((b) >> 32) * ((a)&BIT_FLAG_32))>>32) \
                    + ( \
                        (((((a) >> 32) * ((b)&BIT_FLAG_32))&BIT_FLAG_32) \
                        + ((((b) >> 32) * ((a)&BIT_FLAG_32))&BIT_FLAG_32) \
                        + ((((a)&BIT_FLAG_32) * ((b)&BIT_FLAG_32)) >> 32) ) >> 32 \
                    ) )

static const uint64_t PRIME32_1 = 0x9E3779B1U;
static const uint64_t PRIME32_2 = 0x85EBCA77U;
static const uint64_t PRIME32_3 = 0xC2B2AE3DU;
static const uint64_t PRIME64_1 = 0x9E3779B185EBCA87ULL;
static const uint64_t PRIME64_2 = 0xC2B2AE3D27D4EB4FULL;
static const uint64_t PRIME64_3 = 0x165667B19E3779F9ULL;
static const uint64_t PRIME64_4 = 0x85EBCA77C2B2AE63ULL;
static const uint64_t PRIME64_5 = 0x27D4EB2F165667C5ULL;
static const uint64_t PRIME_MX1 = 0x165667919E3779F9ULL;
static const uint64_t PRIME_MX2 = 0x9FB21C651E98DF25ULL;

static const uint8_t XXH3_defaultSecret[192] = {
  0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe,
  0x7c, 0x01, 0x81, 0x2c, 0xf7, 0x21, 0xad, 0x1c,
  0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90, 0x97, 0xdb,
  0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f,
  0xcb, 0x79, 0xe6, 0x4e, 0xcc, 0xc0, 0xe5, 0x78,
  0x82, 0x5a, 0xd0, 0x7d, 0xcc, 0xff, 0x72, 0x21,
  0xb8, 0x08, 0x46, 0x74, 0xf7, 0x43, 0x24, 0x8e,
  0xe0, 0x35, 0x90, 0xe6, 0x81, 0x3a, 0x26, 0x4c,
  0x3c, 0x28, 0x52, 0xbb, 0x91, 0xc3, 0x00, 0xcb,
  0x88, 0xd0, 0x65, 0x8b, 0x1b, 0x53, 0x2e, 0xa3,
  0x71, 0x64, 0x48, 0x97, 0xa2, 0x0d, 0xf9, 0x4e,
  0x38, 0x19, 0xef, 0x46, 0xa9, 0xde, 0xac, 0xd8,
  0xa8, 0xfa, 0x76, 0x3f, 0xe3, 0x9c, 0x34, 0x3f,
  0xf9, 0xdc, 0xbb, 0xc7, 0xc7, 0x0b, 0x4f, 0x1d,
  0x8a, 0x51, 0xe0, 0x4b, 0xcd, 0xb4, 0x59, 0x31,
  0xc8, 0x9f, 0x7e, 0xc9, 0xd9, 0x78, 0x73, 0x64,
  0xea, 0xc5, 0xac, 0x83, 0x34, 0xd3, 0xeb, 0xc3,
  0xc5, 0x81, 0xa0, 0xff, 0xfa, 0x13, 0x63, 0xeb,
  0x17, 0x0d, 0xdd, 0x51, 0xb7, 0xf0, 0xda, 0x49,
  0xd3, 0x16, 0x55, 0x26, 0x29, 0xd4, 0x68, 0x9e,
  0x2b, 0x16, 0xbe, 0x58, 0x7d, 0x47, 0xa1, 0xfc,
  0x8f, 0xf8, 0xb8, 0xd1, 0x7a, 0xd0, 0x31, 0xce,
  0x45, 0xcb, 0x3a, 0x8f, 0x95, 0x16, 0x04, 0x28,
  0xaf, 0xd7, 0xfb, 0xca, 0xbb, 0x4b, 0x40, 0x7e,
};

/**/

/* TODO Implement for custom secret generation from seed
deriveSecret(u64 seed):
  u64 derivedSecret[24] = defaultSecret[0:192];
  for (i = 0; i < 12; i++) {
    derivedSecret[i*2] += seed;
    derivedSecret[i*2+1] -= seed;
  }
  return derivedSecret; // convert to u8[192] (little-endian)
*/

static inline uint32_t read_32_LE(const void* ptr) 
{
    const uint8_t* p = (const uint8_t*)ptr;
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
static inline uint64_t read_64_LE(const void* ptr) 
{
    const uint8_t* p = (const uint8_t*)ptr;
    return (uint64_t)p[0] | ((uint64_t)p[1] << 8) | ((uint64_t)p[2] << 16) | ((uint64_t)p[3] << 24)
         | ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) | ((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}
static inline uint32_t read_32_BE(const void* ptr) 
{
    const uint8_t* p = (const uint8_t*)ptr;
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | ((uint32_t)p[3]);
}
static inline uint64_t read_64_BE(const void* ptr) 
{
    const uint8_t* p = (const uint8_t*)ptr;
    return ((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40) | ((uint64_t)p[3] << 32)
         | ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) | ((uint64_t)p[6] << 8) | ((uint64_t)p[7]);
}

#define XXH3_AVALANCHE(x) ((((x) ^ ((x) >> 37)) * PRIME_MX1) ^ ((((x) ^ ((x) >> 37)) * PRIME_MX1) >> 32))
static inline uint64_t XXH3_avalanche(uint64_t x) 
{
    x = x ^ (x >> 37);
    x = x * PRIME_MX1;
    x = x ^ (x >> 32);
    return x;
}

#define XXH3_AVALANCHE_XXH64(x) ((((((x) ^ ((x) >> 33)) * PRIME64_2) ^ ((((x) ^ ((x) >> 33)) * PRIME64_2) >> 29)) * PRIME64_3) ^ ((((((x) ^ ((x) >> 33)) * PRIME64_2) ^ ((((x) ^ ((x) >> 33)) * PRIME64_2) >> 29)) * PRIME64_3) >> 32))
static inline uint64_t XXH3_avalanche_XXH64(uint64_t x)
{
    x = x ^ (x >> 33);
    x = x * PRIME64_2;
    x = x ^ (x >> 29);
    x = x * PRIME64_3;
    x = x ^ (x >> 32);
    return x;
}

static inline uint64_t XXH3_mix_step_XXH64(const uint64_t data_word_1, const uint64_t data_word_2, const uint64_t secret_word_1, const uint64_t secret_word_2, const uint64_t seed) 
{
    const uint64_t left = data_word_1 ^ (secret_word_1 + seed);
    const uint64_t right = data_word_2 ^ (secret_word_2 - seed);
    return MUL_HI_64_128(left, right) ^ (left * right);
}

#endif
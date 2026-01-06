#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include "../Dynamic Array/dyn_array.h"
#include "../Strings/String.h"

/**
 * @brief Converts a String to a dyn_array of characters.
 * @param str Pointer to the String to be converted.
 * @return Pointer to a dyn_array containing the characters of the String. Returns NULL if the input String is NULL.
 */
static inline dyn_array* string_to_dyn_array(const String* const str)
{
    if (str == NULL) return NULL;

    dyn_array* const dyn_str = new_dyn_array(DYN_ARRAY_CHAR_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);

    for (int i = 0; i < str->str_length; i++)
    {
        char c = str->string[i];
        append_item_dyn_array(dyn_str, &c);
    }

    strip_dyn_array(dyn_str);

    return dyn_str;
}

/**
 * @brief Converts a dyn_array of characters to a String.
 * @param dyn_str Pointer to the dyn_array to be converted. Must be of type DYN_ARRAY_CHAR_TYPE.
 * @return Pointer to a String containing the characters from the dyn_array. Returns NULL if the input dyn_array is NULL or not of type DYN_ARRAY_CHAR_TYPE.
 */
static inline String* dyn_array_char_to_string(const dyn_array* const dyn_str)
{
    if (dyn_str == NULL && dyn_str->type != DYN_ARRAY_CHAR_TYPE) return NULL;

    String* const str = newStringN((char*)dyn_str->data, dyn_str->current_size * dyn_str->item_size);

    return str;
}

static inline uint8_t index_of_most_significant_bit(const uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {
        if (byte & (1 << i)) return i;
    }
    return -1; // no bits set
}

/**
 * @brief Converts a dyn_array of uints to a String in hex representation.
 * @param dyn_str Pointer to the dyn_array to be converted. Must be of type DYN_ARRAY_UINT_64T_TYPE.
 * @param base The numerical base for conversion (e.g., 16 for hexadecimal). Must be at least 2 and at most 64 and a power of 2. (currently 32, 128 unsupported)
 * @return Pointer to a String containing the characters from the dyn_array. Returns NULL if the input dyn_array is NULL or not of type DYN_ARRAY_CHAR_TYPE.
 */
static inline String* dyn_array_to_string_base(const dyn_array* const dyn_str, const uint8_t base)
{
    if (dyn_str == NULL) return NULL;

    String* const str = newEmptyString();

    if (base < 2 || base > 128) return str; // invalid base
    if (base & (base - 1)) return str; // not a power of 2
    if (base == 32 || base == 128) return str; // unsupported bases atm

    char* chars_arr;
    if (base < 64)
    {
        chars_arr = "0123456789abcdefghijklmnopqrstuv";
    }
    else if (base == 64)
    {
        chars_arr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    }
    else
    {
        chars_arr = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒ";
    }

    const uint8_t bits_per_char = (index_of_most_significant_bit(base - 1) + 1);
    const uint8_t filter = (1 << bits_per_char) - 1;
    
    uint16_t buffer = 0;
    uint8_t bits_in_buffer = 0;
    for (int i = 0; i < dyn_str->current_size * dyn_str->item_size; i++)
    {
        const uint8_t byte = (uint8_t)*get_dyn_array_byte(dyn_str, i);
        buffer = (buffer << 8) | byte;
        bits_in_buffer += 8;
        while (bits_in_buffer >= bits_per_char)
        {
            bits_in_buffer -= bits_per_char;
            const uint8_t char_i = (buffer >> bits_in_buffer) & filter;
            appendChar(str, chars_arr[char_i]);
        }
    }
    if (bits_in_buffer > 0)
    {
        const uint8_t char_i = buffer & filter;
        appendChar(str, chars_arr[char_i]);
    }
    return str;
}

/**
 * @brief Converts a uint8_t into a String using @p base notation.
 * @param value the uint8_t value to convert.
 * @param base The numerical base for conversion (e.g., 16 for hexadecimal). Must be at least 2 and at most 128 and a power of 2.
 * @return Returns the created String with the value, returns NULL if error.
 * @warning The caller is responsible for freeing the returned String.
 */
static inline String* uint8_to_string_base(const uint8_t value, const uint8_t base)
{
    if (base < 2 || base > 128) return NULL; // invalid base
    if (base & (base - 1)) return NULL; // not a power of 2
    if (base == 32 || base == 128) return NULL; // unsupported bases atm
    
    String* const str = newEmptyString();

    char* chars_arr;
    if (base < 64)
    {
        chars_arr = "0123456789abcdefghijklmnopqrstuv";
    }
    else if (base == 64)
    {
        chars_arr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    }
    else
    {
        chars_arr = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒ";
    }

    const uint8_t bits_per_char = (index_of_most_significant_bit(base - 1) + 1);
    const uint8_t filter = (1 << bits_per_char) - 1;
    
    uint16_t buffer = 0;
    uint8_t bits_in_buffer = 0;
    for (int i = 0; i < sizeof(uint8_t); i++)
    {
        const uint8_t byte = (uint8_t)((value >> ((sizeof(uint8_t)-1)*8 - i*8)) & 0xFF);
        buffer = (buffer << 8) | byte;
        bits_in_buffer += 8;
        while (bits_in_buffer >= bits_per_char)
        {
            bits_in_buffer -= bits_per_char;
            const uint8_t char_i = (buffer >> bits_in_buffer) & filter;
            appendChar(str, chars_arr[char_i]);
        }
    }
    if (bits_in_buffer > 0)
    {
        const uint8_t char_i = buffer & filter;
        appendChar(str, chars_arr[char_i]);
    }
    return str;
}

/**
 * @brief Converts a uint16_t into a String using @p base notation.
 * @param value the uint16_t value to convert.
 * @param base The numerical base for conversion (e.g., 16 for hexadecimal). Must be at least 2 and at most 128 and a power of 2.
 * @return Returns the created String with the value, returns NULL if error.
 * @warning The caller is responsible for freeing the returned String.
 */
static inline String* uint16_to_string_base(const uint16_t value, const uint8_t base)
{
    if (base < 2 || base > 128) return NULL; // invalid base
    if (base & (base - 1)) return NULL; // not a power of 2
    if (base == 32 || base == 128) return NULL; // unsupported bases atm
    
    String* const str = newEmptyString();

    char* chars_arr;
    if (base < 64)
    {
        chars_arr = "0123456789abcdefghijklmnopqrstuv";
    }
    else if (base == 64)
    {
        chars_arr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    }
    else
    {
        chars_arr = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒ";
    }

    const uint8_t bits_per_char = (index_of_most_significant_bit(base - 1) + 1);
    const uint8_t filter = (1 << bits_per_char) - 1;

    uint16_t buffer = 0;
    uint8_t bits_in_buffer = 0;
    for (int i = 0; i < sizeof(uint16_t); i++)
    {
        const uint8_t byte = (uint8_t)((value >> ((sizeof(uint16_t)-1)*8 - i*8)) & 0xFF);
        buffer = (buffer << 8) | byte;
        bits_in_buffer += 8;
        while (bits_in_buffer >= bits_per_char)
        {
            bits_in_buffer -= bits_per_char;
            const uint8_t char_i = (buffer >> bits_in_buffer) & filter;
            appendChar(str, chars_arr[char_i]);
        }
    }
    if (bits_in_buffer > 0)
    {
        const uint8_t char_i = buffer & filter;
        appendChar(str, chars_arr[char_i]);
    }
    return str;
}

/**
 * @brief Converts a uint32_t into a String using @p base notation.
 * @param value the uint32_t value to convert.
 * @param base The numerical base for conversion (e.g., 16 for hexadecimal). Must be at least 2 and at most 128 and a power of 2.
 * @return Returns the created String with the value, returns NULL if error.
 * @warning The caller is responsible for freeing the returned String.
 */
static inline String* uint32_to_string_base(const uint32_t value, const uint8_t base)
{
    if (base < 2 || base > 128) return NULL; // invalid base
    if (base & (base - 1)) return NULL; // not a power of 2
    if (base == 32 || base == 128) return NULL; // unsupported bases atm
    
    String* const str = newEmptyString();

    char* chars_arr;
    if (base < 64)
    {
        chars_arr = "0123456789abcdefghijklmnopqrstuv";
    }
    else if (base == 64)
    {
        chars_arr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    }
    else
    {
        chars_arr = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒ";
    }

    const uint8_t bits_per_char = (index_of_most_significant_bit(base - 1) + 1);
    const uint8_t filter = (1 << bits_per_char) - 1;
    
    uint16_t buffer = 0;
    uint8_t bits_in_buffer = 0;
    for (int i = 0; i < sizeof(uint32_t); i++)
    {
        const uint8_t byte = (uint8_t)((value >> ((sizeof(uint32_t)-1)*8 - i*8)) & 0xFF);
        buffer = (buffer << 8) | byte;
        bits_in_buffer += 8;
        while (bits_in_buffer >= bits_per_char)
        {
            bits_in_buffer -= bits_per_char;
            const uint8_t char_i = (buffer >> bits_in_buffer) & filter;
            appendChar(str, chars_arr[char_i]);
        }
    }
    if (bits_in_buffer > 0)
    {
        const uint8_t char_i = buffer & filter;
        appendChar(str, chars_arr[char_i]);
    }
    return str;
}

/**
 * @brief Converts a uint64_t into a String using @p base notation.
 * @param value the uint64_t value to convert.
 * @param base The numerical base for conversion (e.g., 16 for hexadecimal). Must be at least 2 and at most 128 and a power of 2.
 * @return Returns the created String with the value, returns NULL if error.
 * @warning The caller is responsible for freeing the returned String.
 */
static inline String* uint64_to_string_base(const uint64_t value, const uint8_t base)
{
    if (base < 2 || base > 128) return NULL; // invalid base
    if (base & (base - 1)) return NULL; // not a power of 2
    if (base == 32 || base == 128) return NULL; // unsupported bases atm
    
    String* const str = newEmptyString();

    char* chars_arr;
    if (base < 64)
    {
        chars_arr = "0123456789abcdefghijklmnopqrstuv";
    }
    else if (base == 64)
    {
        chars_arr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    }
    else
    {
        chars_arr = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒ";
    }

    const uint8_t bits_per_char = (index_of_most_significant_bit(base - 1) + 1);
    const uint8_t filter = (1 << bits_per_char) - 1;
    
    uint16_t buffer = 0;
    uint8_t bits_in_buffer = 0;
    for (int i = 0; i < sizeof(uint64_t); i++)
    {
        const uint8_t byte = (uint8_t)((value >> ((sizeof(uint64_t)-1)*8 - i*8)) & 0xFF);
        buffer = (buffer << 8) | byte;
        bits_in_buffer += 8;
        while (bits_in_buffer >= bits_per_char)
        {
            bits_in_buffer -= bits_per_char;
            const uint8_t char_i = (buffer >> bits_in_buffer) & filter;
            appendChar(str, chars_arr[char_i]);
        }
    }
    if (bits_in_buffer > 0)
    {
        const uint8_t char_i = buffer & filter;
        appendChar(str, chars_arr[char_i]);
    }
    return str;
}

static inline char* __strchr__(char* const arr, const char c)
{
    int i = 0;
    while (arr[i] != c)
    {
        if (arr[i] == '\0') return NULL;
        i++;
    }
    return &arr[i];
}

/**
 * @brief Converts a String representing a number in a given base to a uint64_t.
 * @param str Pointer to the String to be converted.
 * @param base The numerical base of the input String (e.g., 16 for hexadecimal). Must be at least 2 and at most 64 and a power of 2. (currently 32, 128 unsupported)
 * @return The uint64_t representation of the String. Returns 0 if the input String is NULL or contains invalid characters.
 * @warning This function does not handle overflow; if the number represented by the String exceeds uint64_t, the result will be truncated.
 * @warning Unverified and untested.
 */
static inline uint64_t string_base_to_uint64(const String* const str, const uint8_t base)
{
    if (str == NULL) return 0;

    char* chars_arr;
    if (base < 64)
    {
        chars_arr = "0123456789abcdefghijklmnopqrstuv";
    }
    else if (base == 64)
    {
        chars_arr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    }
    else
    {
        chars_arr = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒ";
    }

    if (base < 2 || base > 128) return 0; // invalid base
    if (base & (base - 1)) return 0; // not a power of 2
    if (base == 32 || base == 128) return 0; // unsupported bases atm

    const uint8_t bits_per_char = (index_of_most_significant_bit(base - 1) + 1);
    uint64_t result = 0;
    for (int i = 0; i < str->str_length; i++)
    {
        char* pos = __strchr__(chars_arr, str->string[i]);
        if (pos == NULL) continue; // invalid character, skip
        uint8_t char_value = (uint8_t)(pos - chars_arr);
        result = (result << bits_per_char) | char_value;
    }
    return result;
}

static inline dyn_array* int_to_dyn_array(const int value)
{
    dyn_array* const dyn_int = new_dyn_array(DYN_ARRAY_INT_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
    append_item_dyn_array(dyn_int, &value);
    return dyn_int;
}

static inline dyn_array* uint_to_dyn_array(const unsigned int value)
{
    dyn_array* const dyn_uint = new_dyn_array(DYN_ARRAY_UINT_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
    append_item_dyn_array(dyn_uint, &value);
    return dyn_uint;
}

static inline dyn_array* uint8_t_to_dyn_array(const uint8_t value)
{
    dyn_array* const dyn_uint8t = new_dyn_array(DYN_ARRAY_UINT_8T_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
    append_item_dyn_array(dyn_uint8t, &value);
    return dyn_uint8t;
}

static inline dyn_array* uint16_t_to_dyn_array(const uint16_t value)
{
    dyn_array* const dyn_uint16t = new_dyn_array(DYN_ARRAY_UINT_16T_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
    append_item_dyn_array(dyn_uint16t, &value);
    return dyn_uint16t;
}

static inline dyn_array* uint32_t_to_dyn_array(const uint32_t value)
{
    dyn_array* const dyn_uint32t = new_dyn_array(DYN_ARRAY_UINT_32T_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
    append_item_dyn_array(dyn_uint32t, &value);
    return dyn_uint32t;
}

static inline dyn_array* uint64_t_to_dyn_array(const uint64_t value)
{
    dyn_array* const dyn_uint64t = new_dyn_array(DYN_ARRAY_UINT_64T_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
    append_item_dyn_array(dyn_uint64t, &value);
    return dyn_uint64t;
}

static inline dyn_array* ptr_to_dyn_array(const void* const ptr)
{
    dyn_array* const dyn_ptr = new_dyn_array(DYN_ARRAY_UINT_64T_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
    const uint64_t ptr_value = (uint64_t)(uintptr_t)ptr;
    append_item_dyn_array(dyn_ptr, &ptr_value);
    return dyn_ptr;
}

#endif
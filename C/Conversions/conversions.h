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
        const uint8_t byte = *(uint8_t*)get_dyn_array_byte(dyn_str, i);
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

#endif
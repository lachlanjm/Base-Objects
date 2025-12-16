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
        append_item_dyn_array(dyn_str, (void*)c);
    }

    strip_dyn_array(dyn_str);

    return dyn_str;
}

/**
 * @brief Converts a dyn_array of characters to a String.
 * @param dyn_str Pointer to the dyn_array to be converted. Must be of type DYN_ARRAY_CHAR_TYPE.
 * @return Pointer to a String containing the characters from the dyn_array. Returns NULL if the input dyn_array is NULL or not of type DYN_ARRAY_CHAR_TYPE.
 */
static inline String* dyn_array_to_string(const dyn_array* const dyn_str)
{
    if (dyn_str == NULL || dyn_str->type != DYN_ARRAY_CHAR_TYPE) return NULL;

    String* const str = newStringN((char*)dyn_str->data, dyn_str->current_size * dyn_str->item_size);

    return str;
}

#endif
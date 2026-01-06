#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "../Dynamic Array/dyn_array.h"
#include "../Strings/String.h"
#include "../Hashing/xxHash-3-64.h"
#include "../Conversions/conversions.h"

#include <stdint.h>
#include <stdio.h>

#define DICTIONARY_OUTPUT_PTR_BUFFER_SIZE 256

enum dictionary_key_value_type
{
    DICTIONARY_KEY_VALUE_TYPE_STRING,
    DICTIONARY_KEY_VALUE_TYPE_INT, // TODO Need to flush out debugging
    DICTIONARY_KEY_VALUE_TYPE_UINT,
    DICTIONARY_KEY_VALUE_TYPE_UINT8_T,
    DICTIONARY_KEY_VALUE_TYPE_UINT16_T,
    DICTIONARY_KEY_VALUE_TYPE_UINT32_T,
    DICTIONARY_KEY_VALUE_TYPE_UINT64_T,
};

struct dictionary_entry
{
    void* key;
    void* value;

    struct dictionary_entry* next_entry; // For linked list of all entries
    struct dictionary_entry* prev_entry; // For linked list of all entries

    struct dictionary_entry* next_in_bucket;
    struct dictionary_entry* prev_in_bucket;
};

enum dictionary_hash_function
{
    DICTIONARY_HASH_FUNCTION_XXH3
};
#define DICTIONARY_HASH_FUNCTION_DEFAULT (DICTIONARY_HASH_FUNCTION_XXH3)

// TODO implement random seeding
// TODO implement more key/value types; will need to allocate/free memory properly for even basic types
// TODO allow for unspecified key/value types (void*), with user-provided hash and comparison functions (i.e. key_size & value_size parameters, function pointers, etc.)
typedef struct Dictionary
{
    enum dictionary_hash_function hash_function;
    enum dictionary_key_value_type key_type;
    enum dictionary_key_value_type value_type;

    uint16_t array_count;
    uint64_t array_size;
    
    uint64_t* hash_seeds; // or salts
    struct dictionary_entry* first_entry;
    struct dictionary_entry** entries;
} Dictionary;

typedef int(*comparator_func)(const void*, const void*); // return 0 on equality

static inline void set_dictionary(Dictionary* const dict, const uint16_t array_count, const uint64_t array_size, const enum dictionary_hash_function hash_function, const enum dictionary_key_value_type key_type, const enum dictionary_key_value_type value_type)
{
    dict->hash_function = hash_function;
    dict->key_type = key_type;
    dict->value_type = value_type;
    dict->array_count = array_count;
    dict->array_size = array_size;
    dict->first_entry = NULL;

    dict->hash_seeds = (uint64_t*)calloc(array_count, sizeof(uint64_t));
    for (int i = 0; i < array_count; i++)
    {
        dict->hash_seeds[i] = (uint64_t)(i * 2654435761); // Example seed generation
    }

    dict->entries = (struct dictionary_entry**)calloc(array_count * array_size, sizeof(struct dictionary_entry*));
}

static inline Dictionary* new_dictionary(const uint16_t array_count, const uint64_t array_size, const enum dictionary_hash_function hash_function, const enum dictionary_key_value_type key_type, const enum dictionary_key_value_type value_type)
{
    Dictionary* const dict = (Dictionary*)calloc(1, sizeof(Dictionary));
    set_dictionary(dict, array_count, array_size, hash_function, key_type, value_type);
    return dict;
}

static inline Dictionary* new_dictionary_default(const enum dictionary_key_value_type key_type, const enum dictionary_key_value_type value_type)
{
    return new_dictionary(8, 256, DICTIONARY_HASH_FUNCTION_DEFAULT, key_type, value_type);
}

static inline uint64_t compute_hash(const enum dictionary_hash_function hash_function, const uint64_t seed, const dyn_array* const key)
{
    switch (hash_function)
    {
        case DICTIONARY_HASH_FUNCTION_XXH3:
            return digest_XXH3_64_with_seed(key, seed);
        default:
            return compute_hash(DICTIONARY_HASH_FUNCTION_DEFAULT, seed, key);
    }
}

static inline uint64_t compute_index_in_dictionary(const enum dictionary_hash_function hash_function, const uint64_t array_size, const uint64_t seed, const dyn_array* const key)
{
    return compute_hash(hash_function, seed, key) % array_size;
}

/**
 * Converts a key of various types to a dynamic array for hashing.
 * @param key_type The type of the key.
 * @param key Pointer to the key.
 * @return Pointer to a dyn_array representing the key, or NULL if the key type is unsupported.
 * @warning The caller is responsible for freeing the returned dyn_array.
 */
static inline dyn_array* __dictionary_key_to_dyn_array__(const enum dictionary_key_value_type key_type, const void* const key)
{
    switch (key_type)
    {
        case DICTIONARY_KEY_VALUE_TYPE_STRING:
            return string_to_dyn_array((String*)key);
        case DICTIONARY_KEY_VALUE_TYPE_INT:
            return int_to_dyn_array(*(const int*)key);
        case DICTIONARY_KEY_VALUE_TYPE_UINT:
            return uint_to_dyn_array(*(const unsigned int*)key);
        case DICTIONARY_KEY_VALUE_TYPE_UINT8_T:
            return uint8_t_to_dyn_array(*(const uint8_t*)key);
        case DICTIONARY_KEY_VALUE_TYPE_UINT16_T:
            return uint16_t_to_dyn_array(*(const uint16_t*)key);
        case DICTIONARY_KEY_VALUE_TYPE_UINT32_T:
            return uint32_t_to_dyn_array(*(const uint32_t*)key);
        case DICTIONARY_KEY_VALUE_TYPE_UINT64_T:
            return uint64_t_to_dyn_array(*(const uint64_t*)key);
        default:
            return ptr_to_dyn_array(key); // Other key types not implemented yet
    }
}

static inline int __ptr_compare__(const void* a, const void* b)
{
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

static inline int __int_compare__(const void* a, const void* b)
{
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return (int_a > int_b) - (int_a < int_b);
}

static inline int __uint_compare__(const void* a, const void* b)
{
    unsigned int uint_a = *(const unsigned int*)a;
    unsigned int uint_b = *(const unsigned int*)b;
    return (uint_a > uint_b) - (uint_a < uint_b);
}

static inline int __uint8_t_compare__(const void* a, const void* b)
{
    uint8_t uint_a = *(const uint8_t*)a;
    uint8_t uint_b = *(const uint8_t*)b;
    return (uint_a > uint_b) - (uint_a < uint_b);
}

static inline int __uint16_t_compare__(const void* a, const void* b)
{
    uint16_t uint_a = *(const uint16_t*)a;
    uint16_t uint_b = *(const uint16_t*)b;
    return (uint_a > uint_b) - (uint_a < uint_b);
}

static inline int __uint32_t_compare__(const void* a, const void* b)
{
    uint32_t uint_a = *(const uint32_t*)a;
    uint32_t uint_b = *(const uint32_t*)b;
    return (uint_a > uint_b) - (uint_a < uint_b);
}

static inline int __uint64_t_compare__(const void* a, const void* b)
{
    uint64_t uint_a = *(const uint64_t*)a;
    uint64_t uint_b = *(const uint64_t*)b;
    return (uint_a > uint_b) - (uint_a < uint_b);
}

static inline int __String_compare__(const void* a, const void* b)
{
    return compareString((const String*)a, (const String*)b);
}

static inline comparator_func __get_dictionary_key_compare_function__(const enum dictionary_key_value_type key_type)
{
    switch (key_type)
    {
        case DICTIONARY_KEY_VALUE_TYPE_STRING:
            return &__String_compare__;
        case DICTIONARY_KEY_VALUE_TYPE_INT:
            return &__int_compare__;
        case DICTIONARY_KEY_VALUE_TYPE_UINT:
            return &__uint_compare__;
        case DICTIONARY_KEY_VALUE_TYPE_UINT8_T:
            return &__uint8_t_compare__;
        case DICTIONARY_KEY_VALUE_TYPE_UINT16_T:
            return &__uint16_t_compare__;
        case DICTIONARY_KEY_VALUE_TYPE_UINT32_T:
            return &__uint32_t_compare__;
        case DICTIONARY_KEY_VALUE_TYPE_UINT64_T:
            return &__uint64_t_compare__;
        default:
            return &__ptr_compare__;
    }
}

/**
 * Retrieves the value associated with a given key in the dictionary.
 * @param dict Pointer to the dictionary.
 * @param key Pointer to the key.
 * @return Pointer to the value associated with the key, or NULL if the key is not found.
 */
static inline void* get_value_dictionary(const Dictionary* const dict, const void* const key)
{
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key);
    const comparator_func key_compare_func = __get_dictionary_key_compare_function__(dict->key_type);
    
    for (int i = 0; i < dict->array_count; i++)
    {
        const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
        const uint64_t entry_index = i * dict->array_size + index;

        struct dictionary_entry* entry = dict->entries[entry_index];
        while (entry != NULL)
        {
            if (key_compare_func(entry->key, key) == 0)
            {
                free_dyn_array(key_array);
                return entry->value;
            }
            entry = entry->next_in_bucket;
        }
    }

    free_dyn_array(key_array);
    return NULL;
}

/**
 * Inserts a key-value pair into the dictionary.
 * @param dict Pointer to the dictionary.
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 * @return Returns 0 on success, else error (1 duplicate key found; 2 other)
 * @warning This only shallow-copies the key and value pointers; proper memory management is required by the user outside of the structure.
 */
static inline uint8_t insert_key_value_pair_dictionary(Dictionary* const dict, const void* const key, const void* const value)
{
    // Implementation for inserting key-value pair
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key);
    const comparator_func key_compare_func = __get_dictionary_key_compare_function__(dict->key_type);

    struct dictionary_entry* new_entry = (struct dictionary_entry*)calloc(1, sizeof(struct dictionary_entry));
    new_entry->key = (void*)key;
    new_entry->value = (void*)value;

    uint8_t return_code = 2;
    uint8_t added_entry = 0;

    uint64_t min_entry_stack = 0;
    uint16_t min_entry_stack_array = 0;
    uint64_t min_entry_stack_index = 0;
    for (int i = 0; i < dict->array_count; i++)
    {
        const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
        const uint64_t entry_index = i * dict->array_size + index;

        // Count entries in this bucket
        uint64_t entry_stack = 0;
        struct dictionary_entry* entry = dict->entries[entry_index];
        while (entry != NULL)
        {
            if (key_compare_func(entry->key, key) == 0)
            {
                return_code = 1;
                break;
            }

            entry_stack++;
            entry = entry->next_in_bucket;
        }
        if (return_code == 1) break;

        if (entry_stack < min_entry_stack || i == 0)
        {
            min_entry_stack = entry_stack;
            min_entry_stack_array = i;
            min_entry_stack_index = index;
        }
    }

    if (added_entry == 0 && return_code != 1)
    {
        // Insert into the least filled bucket
        const uint64_t entry_index = min_entry_stack_array * dict->array_size + min_entry_stack_index;

        if (min_entry_stack == 0)
        {
            // Empty bucket, insert here
            dict->entries[entry_index] = new_entry;
        }
        else
        {
            // Maybe look into sorted insertion later or something
            // Insert into bucket (simple chaining)
            new_entry->next_in_bucket = dict->entries[entry_index];
            dict->entries[entry_index]->prev_in_bucket = new_entry;
            dict->entries[entry_index] = new_entry;
        }

        added_entry = 1;
        return_code = 0;
    }
    
    free_dyn_array(key_array);

    if (added_entry)
    {
        new_entry->next_entry = dict->first_entry;
        if (dict->first_entry != NULL)
        {
            dict->first_entry->prev_entry = new_entry;
        }
        dict->first_entry = new_entry;
    }

    return return_code;
}

/**
 * Retrieves the value associated with a given key in the dictionary.
 * @param dict Pointer to the dictionary.
 * @param key Pointer to the key to update.
 * @param value Pointer to the new value.
 * @return Returns 0 on success, else error (1 key not found; 2 other)
 */
static inline uint8_t set_value_dictionary(const Dictionary* const dict, const void* const key, const void* const value)
{
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key);
    const comparator_func key_compare_func = __get_dictionary_key_compare_function__(dict->key_type);

    uint8_t key_found = 0;
    uint8_t return_code = 2;
    
    int i = 0;
    for (; i < dict->array_count; i++)
    {
        const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
        const uint64_t entry_index = i * dict->array_size + index;

        struct dictionary_entry* entry = dict->entries[entry_index];
        while (entry != NULL)
        {
            if (key_compare_func(entry->key, key) == 0)
            {
                key_found = 1;

                entry->value = value;

                return_code = 0;
                break;
            }
            entry = entry->next_in_bucket;
        }
    }
    if (key_found == 0 && i == dict->array_count)
    {
        return_code = 1;
    }

    free_dyn_array(key_array);
    return return_code;
}

/**
 * Deletes a key-value pair from the dictionary by key.
 * @param dict Pointer to the dictionary.
 * @param key Pointer to the key to delete.
 * @warning This is a simplified version and does not handle all edge cases.
 */
static inline void delete_key_value_pair_dictionary(Dictionary* const dict, const void* const key)
{
    // Implementation for deleting key-value pair by key
    // Note: This is a simplified version and does not handle all edge cases
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key);
    const comparator_func key_compare_func = __get_dictionary_key_compare_function__(dict->key_type);

    for (int i = 0; i < dict->array_count; i++)
    {
        const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
        const uint64_t entry_index = i * dict->array_size + index;

        struct dictionary_entry* entry = dict->entries[entry_index];
        while (entry != NULL)
        {
            if (key_compare_func(entry->key, key) == 0)
            {
                // Found the entry to delete
                if (entry->prev_in_bucket != NULL)
                {
                    entry->prev_in_bucket->next_in_bucket = entry->next_in_bucket;
                }
                else
                {
                    dict->entries[entry_index] = entry->next_in_bucket;
                }
                if (entry->next_in_bucket != NULL)
                {
                    entry->next_in_bucket->prev_in_bucket = entry->prev_in_bucket;
                }

                // Remove from linked list of all entries
                if (entry->prev_entry != NULL)
                {
                    entry->prev_entry->next_entry = entry->next_entry;
                }
                else
                {
                    dict->first_entry = entry->next_entry;
                }
                if (entry->next_entry != NULL)
                {
                    entry->next_entry->prev_entry = entry->prev_entry;
                }

                free(entry);
                free_dyn_array(key_array);
                return;
            }
            entry = entry->next_in_bucket;
        }
    }

    free_dyn_array(key_array);
}

/**
 * Generates a string representation of all key-value pairs in the dictionary.
 * @param dict Pointer to the dictionary.
 * @return Pointer to a String containing the key-value pairs.
 * @warning The caller is responsible for freeing the returned String.
 */
static inline String* get_dictionary_entries_key_values_string(const Dictionary* const dict)
{
    String* result = newString("");

    char buf[DICTIONARY_OUTPUT_PTR_BUFFER_SIZE];
    String* value_buf;

    struct dictionary_entry* entry = dict->first_entry;
    while (entry != NULL)
    {
        appendCharsN(result, "Key: ", 5);
        switch (dict->key_type)
        {
            case DICTIONARY_KEY_VALUE_TYPE_STRING:
                appendString(result, (String*)entry->key);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT:
                value_buf = uint64_to_string_base((uint64_t)*((unsigned int*)entry->key), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT8_T:
                value_buf = uint8_to_string_base(*((uint8_t*)entry->key), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT16_T:
                value_buf = uint16_to_string_base(*((uint16_t*)entry->key), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT32_T:
                value_buf = uint32_to_string_base(*((uint32_t*)entry->key), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT64_T:
                value_buf = uint64_to_string_base(*((uint64_t*)entry->key), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            
            case DICTIONARY_KEY_VALUE_TYPE_INT:
            default:
                snprintf(buf, DICTIONARY_OUTPUT_PTR_BUFFER_SIZE, "%p", entry->key);
                appendChars(result, buf);
                break;
        }
        appendCharsN(result, ", Value: ", 9);
        switch (dict->value_type)
        {
            case DICTIONARY_KEY_VALUE_TYPE_STRING:
                appendString(result, (String*)entry->value);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT:
                value_buf = uint64_to_string_base((uint64_t)*((unsigned int*)entry->value), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT8_T:
                value_buf = uint8_to_string_base(*((uint8_t*)entry->value), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT16_T:
                value_buf = uint16_to_string_base(*((uint16_t*)entry->value), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT32_T:
                value_buf = uint32_to_string_base(*((uint32_t*)entry->value), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            case DICTIONARY_KEY_VALUE_TYPE_UINT64_T:
                value_buf = uint64_to_string_base(*((uint64_t*)entry->value), 16);
                appendString(result, value_buf);
                freeString(value_buf);
                break;
            
            case DICTIONARY_KEY_VALUE_TYPE_INT:
            default:
                snprintf(buf, DICTIONARY_OUTPUT_PTR_BUFFER_SIZE, "%p", entry->value);
                appendChars(result, buf);
                break;
        }
        appendCharsN(result, "\n", 1);
        entry = entry->next_entry;
    }
    return result;
}

static inline void clean_dictionary(Dictionary* const dict)
{
    if (dict->hash_seeds != NULL) free(dict->hash_seeds);
    dict->hash_seeds = NULL;

    if (dict->entries != NULL)
    {
        struct dictionary_entry* entry = dict->first_entry;
        while (entry != NULL)
        {
            struct dictionary_entry* const next_entry = entry->next_entry;
            free(entry);
            entry = next_entry;
        }
        free(dict->entries);
    }
    dict->entries = NULL;
}

static inline void free_dictionary(Dictionary* const dict)
{
    clean_dictionary(dict);
    free(dict);
}


#endif
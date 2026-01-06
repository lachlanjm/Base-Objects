#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "../Dynamic Array/dyn_array.h"
#include "../Strings/String.h"
#include "../Hashing/xxHash-3-64.h"
#include "../Conversions/conversions.h"

#include <stdint.h>

enum dictionary_key_value_type
{
    DICTIONARY_KEY_VALUE_TYPE_STRING,
    DICTIONARY_KEY_VALUE_TYPE_INT, // need to implement
    DICTIONARY_KEY_VALUE_TYPE_UINT, // need to implement
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

static inline void set_dictionary(Dictionary* const dict, const uint16_t array_count, const uint64_t array_size, const enum dictionary_hash_function hash_function, const enum dictionary_key_value_type key_type, const enum dictionary_key_value_type value_type)
{
    dict->hash_function = hash_function;
    dict->key_type = key_type;
    dict->value_type = value_type;
    dict->array_count = array_count;
    dict->array_size = array_size;

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

static inline void* __get_value_dictionary_key_string__(const Dictionary* const dict, const String* const key)
{
    // Implementation for retrieving value by string key
    dyn_array* key_array = string_to_dyn_array(key);
    
    for (int i = 0; i < dict->array_count; i++)
    {
        const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
        const uint64_t entry_index = i * dict->array_size + index;

        struct dictionary_entry* entry = dict->entries[entry_index];
        while (entry != NULL)
        {
            String* entry_key = (String*)entry->key;
            if (compareString(entry_key, key) == 0)
            {
                free_dyn_array((dyn_array*)key_array);
                return entry->value;
            }
            entry = entry->next_in_bucket;
        }
    }

    free_dyn_array(key_array);
    return NULL;
}

static inline void* get_value_dictionary(const Dictionary* const dict, const void* const key)
{
    switch (dict->key_type)
    {
        case DICTIONARY_KEY_VALUE_TYPE_STRING:
            return __get_value_dictionary_key_string__(dict, (String*)key);
        default:
            return NULL; // Other key types not implemented yet
    }
}

static inline void clean_dictionary(Dictionary* const dict)
{
    if (dict->hash_seeds != NULL) free(dict->hash_seeds);
    dict->hash_seeds = NULL;

    if (dict->entries != NULL)
    {
        for (int i = 0; i < dict->array_count * dict->array_size; i++)
        {
            struct dictionary_entry* entry = dict->entries[i];
            while (entry != NULL)
            {
                struct dictionary_entry* next_entry = entry->next_in_bucket;

                switch (dict->key_type)
                {
                    case DICTIONARY_KEY_VALUE_TYPE_STRING:
                        String* key_str = (String*)entry->key;
                        if (key_str != NULL) freeString(key_str);
                        break;
                    default:
                        break; // Other key types not implemented yet
                }

                switch (dict->value_type)
                {
                    case DICTIONARY_KEY_VALUE_TYPE_STRING:
                        String* value_str = (String*)entry->value;
                        if (value_str != NULL) freeString(value_str);
                        break;
                    default:
                        break; // Other key types not implemented yet
                }

                free(entry);
                entry = next_entry;
            }
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
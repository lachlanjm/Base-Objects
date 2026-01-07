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
    DICTIONARY_KEY_VALUE_TYPE_VECTOR_2,  // TODO Need to flush out debugging
	DICTIONARY_KEY_VALUE_TYPE_VECTOR_3,  // TODO Need to flush out debugging
	DICTIONARY_KEY_VALUE_TYPE_VECTOR_4,  // TODO Need to flush out debugging
	DICTIONARY_KEY_VALUE_TYPE_MATRIX_2X2, // TODO Need to flush out debugging
	DICTIONARY_KEY_VALUE_TYPE_MATRIX_3X3, // TODO Need to flush out debugging
	DICTIONARY_KEY_VALUE_TYPE_MATRIX_4X4, // TODO Need to flush out debugging
	DICTIONARY_KEY_VALUE_TYPE_CUSTOM, // TODO Need to flush out debugging
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

enum dictionary_copy_type
{
    DICTIONARY_SHALLOW_COPY,
    DICTIONARY_DEEP_COPY,
};

typedef int(*comparator_func)(const void*, const void*); // return 0 on equality
typedef void(*cleanup_func)(void*);
typedef uint8_t(*copy_func)(const void*, void*); // src, dst

// TODO check the custom type works (deep and shallow copies)
typedef struct Dictionary
{
    enum dictionary_hash_function hash_function;
    enum dictionary_key_value_type key_type;
    enum dictionary_key_value_type value_type;
    enum dictionary_copy_type copy_type;

    uint64_t key_size;
    uint64_t value_size;
    copy_func key_copy_func;
    copy_func value_copy_func;
    cleanup_func key_cleanup_func; // The function that needs to occur before the key item is freed (i.e. free internal allocations)
    cleanup_func value_cleanup_func; // The function that needs to occur before the value item is freed (i.e. free internal allocations)

    uint16_t array_count;
    uint64_t array_size;
    
    uint64_t* hash_seeds; // or salts
    struct dictionary_entry* first_entry;
    struct dictionary_entry** entries;
} Dictionary;

static inline uint64_t __get_type_size__(const enum dictionary_key_value_type type)
{
    switch (type)
    {
		case DICTIONARY_KEY_VALUE_TYPE_STRING:
			return sizeof(String);
        case DICTIONARY_KEY_VALUE_TYPE_INT:
			return sizeof(int);
		case DICTIONARY_KEY_VALUE_TYPE_UINT:
			return sizeof(unsigned int);
		case DICTIONARY_KEY_VALUE_TYPE_UINT8_T:
			return sizeof(uint8_t);
		case DICTIONARY_KEY_VALUE_TYPE_UINT16_T:
			return sizeof(uint16_t);
		case DICTIONARY_KEY_VALUE_TYPE_UINT32_T:
			return sizeof(uint32_t);
		case DICTIONARY_KEY_VALUE_TYPE_UINT64_T:
			return sizeof(uint64_t);
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_2:
			return sizeof(vector2);
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_3:
			return sizeof(vector3);
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_4:
			return sizeof(vector4);
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_2X2:
			return sizeof(matrix_2x2);
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_3X3:
			return sizeof(matrix_3x3);
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_4X4:
			return sizeof(matrix_4x4);
		default:
			return sizeof(char*);
    }
}

static inline uint8_t __copy_String__(const void* src, void* dst) 
{ writeString((String*) dst, (String*) src); return 0; }
static inline uint8_t __copy_int__(const void* src, void* dst) 
{ *(int*)dst = *(int*)src; return 0; }
static inline uint8_t __copy_uint__(const void* src, void* dst) 
{ *(unsigned int*)dst = *(unsigned int*)src; return 0; }
static inline uint8_t __copy_uint8_t__(const void* src, void* dst) 
{ *(uint8_t*)dst = *(uint8_t*)src; return 0; }
static inline uint8_t __copy_uint16_t__(const void* src, void* dst) 
{ *(uint16_t*)dst = *(uint16_t*)src; return 0; }
static inline uint8_t __copy_uint32_t__(const void* src, void* dst) 
{ *(uint32_t*)dst = *(uint32_t*)src; return 0; }
static inline uint8_t __copy_uint64_t__(const void* src, void* dst) 
{ *(uint64_t*)dst = *(uint64_t*)src; return 0; }
static inline uint8_t __copy_vector_2__(const void* src, void* dst) 
{ for (int i = 0; i < 2; i++) ((vector2*)dst)->arr[i] = ((vector2*)src)->arr[i]; return 0; }
static inline uint8_t __copy_vector_3__(const void* src, void* dst) 
{ for (int i = 0; i < 3; i++) ((vector3*)dst)->arr[i] = ((vector3*)src)->arr[i]; return 0; }
static inline uint8_t __copy_vector_4__(const void* src, void* dst) 
{ for (int i = 0; i < 4; i++) ((vector4*)dst)->arr[i] = ((vector4*)src)->arr[i]; return 0; }
static inline uint8_t __copy_matrix_2x2__(const void* src, void* dst) 
{ for (int i = 0; i < 2*2; i++) ((matrix_2x2*)dst)->arr[i] = ((matrix_2x2*)src)->arr[i]; return 0; }
static inline uint8_t __copy_matrix_3x3__(const void* src, void* dst) 
{ for (int i = 0; i < 3*3; i++) ((matrix_3x3*)dst)->arr[i] = ((matrix_3x3*)src)->arr[i]; return 0; }
static inline uint8_t __copy_matrix_4x4__(const void* src, void* dst) 
{ for (int i = 0; i < 4*4; i++) ((matrix_4x4*)dst)->arr[i] = ((matrix_4x4*)src)->arr[i]; return 0; }

static inline copy_func __get_copy_func__(const enum dictionary_key_value_type type)
{
    switch (type)
    {
		case DICTIONARY_KEY_VALUE_TYPE_STRING:
			return __copy_String__;
        case DICTIONARY_KEY_VALUE_TYPE_INT:
			return __copy_int__;
		case DICTIONARY_KEY_VALUE_TYPE_UINT:
			return __copy_uint__;
		case DICTIONARY_KEY_VALUE_TYPE_UINT8_T:
			return __copy_uint8_t__;
		case DICTIONARY_KEY_VALUE_TYPE_UINT16_T:
			return __copy_uint16_t__;
		case DICTIONARY_KEY_VALUE_TYPE_UINT32_T:
			return __copy_uint32_t__;
		case DICTIONARY_KEY_VALUE_TYPE_UINT64_T:
			return __copy_uint64_t__;
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_2:
			return __copy_vector_2__;
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_3:
			return __copy_vector_3__;
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_4:
			return __copy_vector_4__;
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_2X2:
			return __copy_matrix_2x2__;
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_3X3:
			return __copy_matrix_3x3__;
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_4X4:
			return __copy_matrix_4x4__;
		default:
			return NULL;
    }
}

static inline void __clean_String__(void* string) {return cleanString((String*) string);}
static inline void __clean_empty__(void* string) {return;} // For when cleaning is not needed

static inline cleanup_func __get_type_cleanup_func__(const enum dictionary_key_value_type type)
{
    switch (type)
    {
		case DICTIONARY_KEY_VALUE_TYPE_STRING:
			return __clean_String__;
        case DICTIONARY_KEY_VALUE_TYPE_INT:
		case DICTIONARY_KEY_VALUE_TYPE_UINT:
        case DICTIONARY_KEY_VALUE_TYPE_UINT8_T:
        case DICTIONARY_KEY_VALUE_TYPE_UINT16_T:
        case DICTIONARY_KEY_VALUE_TYPE_UINT32_T:
        case DICTIONARY_KEY_VALUE_TYPE_UINT64_T:
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_2:
        case DICTIONARY_KEY_VALUE_TYPE_VECTOR_3:
        case DICTIONARY_KEY_VALUE_TYPE_VECTOR_4:
        case DICTIONARY_KEY_VALUE_TYPE_MATRIX_2X2:
        case DICTIONARY_KEY_VALUE_TYPE_MATRIX_3X3:
        case DICTIONARY_KEY_VALUE_TYPE_MATRIX_4X4:
		default:
			return __clean_empty__;
    }
}

/**
 * @brief Initialize a pre-allocated Dictionary structure: set metadata, allocate hash seeds and bucket table, and prepare for use.
 * @param dict Pointer to an existing Dictionary object to initialize. The caller must allocate the Dictionary (e.g., with calloc) before calling.
 * @param array_count Number of hash arrays (i.e., independent hash seeds / tables). Determines how many hash functions/seeds are used.
 * @param array_size Number of buckets per array. The final bucket table size is @p array_count * @p array_size.
 * @param hash_function Hash algorithm selection (e.g., DICTIONARY_HASH_FUNCTION_XXH3).
 * @param key_type Key data type (enum dictionary_key_value_type). Affects key hashing and comparisons.
 * @param value_type Value data type (enum dictionary_key_value_type). Affects printing/formatting in helper functions
 * @param copy_type Copy behavior (DICTIONARY_SHALLOW_COPY or DICTIONARY_DEEP_COPY). If invalid value is given, DICTIONARY_SHALLOW_COPY is defaulted.
 * @param custom_key_size Byte size for custom key type when @p key_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM.
 * @param custom_value_size Byte size for custom value type when @p value_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM.
 * @param custom_key_cleanup_func Optional cleanup function for custom keys (stored for future use).
 * @param custom_value_cleanup_func Optional cleanup function for custom values (stored for future use).
 * @warning If DICTIONARY_KEY_VALUE_TYPE_CUSTOM is given, the corresponding size and cleanup_func must be provided. These values will be ignored if DICTIONARY_KEY_VALUE_TYPE_CUSTOM is not given.
 */
static inline void set_dictionary(
    Dictionary* const dict, 
    const uint16_t array_count, const uint64_t array_size, 
    const enum dictionary_hash_function hash_function, 
    const enum dictionary_key_value_type key_type, 
    const enum dictionary_key_value_type value_type,
    const enum dictionary_copy_type copy_type,
    const uint64_t custom_key_size,
    const uint64_t custom_value_size,
    const copy_func custom_key_copy_func,
    const copy_func custom_value_copy_func,
    const cleanup_func custom_key_cleanup_func,
    const cleanup_func custom_value_cleanup_func
) {
    dict->hash_function = hash_function;
    dict->key_type = key_type;
    dict->value_type = value_type;
    dict->array_count = array_count;
    dict->array_size = array_size;
    dict->first_entry = NULL;

    // Makes sure is valid
    switch (copy_type)
    {
        case DICTIONARY_DEEP_COPY:
        case DICTIONARY_SHALLOW_COPY:
            dict->copy_type = copy_type;
            break;
        default:
            dict->copy_type = DICTIONARY_SHALLOW_COPY;
            break;
    }

    if (dict->key_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM)
    {
        dict->key_size = custom_key_size;
        dict->key_copy_func = custom_key_copy_func;
        dict->key_cleanup_func = custom_key_cleanup_func;
    }
    else
    {
        dict->key_size = __get_type_size__(dict->key_type);
        dict->key_copy_func = __get_copy_func__(dict->key_type);
        dict->key_cleanup_func = __get_type_cleanup_func__(dict->key_type);
    }

    if (dict->value_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM)
    {
        dict->value_size = custom_value_size;
        dict->value_copy_func = custom_value_copy_func;
        dict->value_cleanup_func = custom_value_cleanup_func;
    }
    else
    {
        dict->value_size = __get_type_size__(dict->value_type);
        dict->value_copy_func = __get_copy_func__(dict->value_type);
        dict->value_cleanup_func = __get_type_cleanup_func__(dict->value_type);
    }

    dict->hash_seeds = (uint64_t*)calloc(array_count, sizeof(uint64_t));
    for (uint64_t i = 17; i < array_count; i++) // 17 is just a random number
    {
        // doesn't need to be random, since don't need cryptographic security, and is ok for practice (maybe not theory)
        dict->hash_seeds[i] = i;
    }

    dict->entries = (struct dictionary_entry**)calloc(array_count * array_size, sizeof(struct dictionary_entry*));
}

static inline Dictionary* new_dictionary(
    const uint16_t array_count, const uint64_t array_size, 
    const enum dictionary_hash_function hash_function, 
    const enum dictionary_key_value_type key_type, 
    const enum dictionary_key_value_type value_type,
    const enum dictionary_copy_type copy_type,
    const uint64_t custom_key_size,
    const uint64_t custom_value_size,
    const copy_func custom_key_copy_func,
    const copy_func custom_value_copy_func,
    const cleanup_func custom_key_cleanup_func,
    const cleanup_func custom_value_cleanup_func
) {
    Dictionary* const dict = (Dictionary*)calloc(1, sizeof(Dictionary));
    set_dictionary(dict, array_count, array_size, hash_function, key_type, value_type, copy_type, 
        custom_key_size, custom_value_size, custom_key_copy_func, custom_value_copy_func, custom_key_cleanup_func, custom_value_cleanup_func);
    return dict;
}

/**
 * @brief creates a new dictionary object with some default parameters, can not call DICTIONARY_KEY_VALUE_TYPE_CUSTOM for either type.
 */
static inline Dictionary* new_dictionary_default(const enum dictionary_key_value_type key_type, const enum dictionary_key_value_type value_type)
{
    if (key_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM || value_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM)
    {
        return NULL;
    }

    return new_dictionary(8, 256, DICTIONARY_HASH_FUNCTION_DEFAULT, key_type, value_type, DICTIONARY_DEEP_COPY, 0, 0, NULL, NULL, NULL, NULL);
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
static inline dyn_array* __dictionary_key_to_dyn_array__(const enum dictionary_key_value_type key_type, const void* const key, const uint64_t custom_key_size)
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
        case DICTIONARY_KEY_VALUE_TYPE_VECTOR_2:
			return vector2_to_dyn_array((vector2*)key);
        case DICTIONARY_KEY_VALUE_TYPE_VECTOR_3:
			return vector3_to_dyn_array((vector3*)key);
		case DICTIONARY_KEY_VALUE_TYPE_VECTOR_4:
			return vector3_to_dyn_array((vector3*)key);
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_2X2:
			return matrix_2x2_to_dyn_array((matrix_2x2*)key);
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_3X3:
			return matrix_3x3_to_dyn_array((matrix_3x3*)key);
		case DICTIONARY_KEY_VALUE_TYPE_MATRIX_4X4:
			return matrix_4x4_to_dyn_array((matrix_4x4*)key);
        case DICTIONARY_KEY_VALUE_TYPE_CUSTOM:
            dyn_array* key_array = new_dyn_array(DYN_ARRAY_UINT_8T_TYPE, DYN_ARRAY_EXPANSION_DOUBLE);
            for (int i = 0; i < custom_key_size; i++)
            {
                append_item_dyn_array(key_array, &((uint8_t*)key)[i]);
            }
            return key_array;
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

static inline int __custom_compare__(const void* a, const void* b, const uint64_t byte_size)
{
    for (int i = 0; i < byte_size; i++)
    {
        const uint8_t a = ((uint8_t*)a)[i];
        const uint8_t b = ((uint8_t*)b)[i];
        if (a - b) return (a > b) - (a < b);
    }
    return 0;
}

/**
 * Retrieves the value associated with a given key in the dictionary.
 * @param dict Pointer to the dictionary.
 * @param key Pointer to the key.
 * @return Pointer to the value associated with the key, or NULL if the key is not found.
 */
static inline void* get_value_dictionary(const Dictionary* const dict, const void* const key)
{
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key, dict->key_size);
    if (dict->key_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM)
    {
        const uint64_t custom_key_size = dict->key_size;
        
        for (int i = 0; i < dict->array_count; i++)
        {
            const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
            const uint64_t entry_index = i * dict->array_size + index;
    
            struct dictionary_entry* entry = dict->entries[entry_index];
            while (entry != NULL)
            {
                if (__custom_compare__(entry->key, key, custom_key_size) == 0)
                {
                    free_dyn_array(key_array);
                    return entry->value;
                }
                entry = entry->next_in_bucket;
            }
        }
    }
    else
    {
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
    }

    free_dyn_array(key_array);
    return NULL;
}

/**
 * Inserts a key-value pair into the dictionary.
 * @param dict Pointer to the dictionary.
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 * @return Returns 0 on success, else error (1 duplicate key found; 2 deep copying error)
 * @warning This only shallow-copies the key and value pointers; proper memory management is required by the user outside of the structure.
 */
static inline uint8_t insert_key_value_pair_dictionary(Dictionary* const dict, const void* const key, const void* const value)
{
    uint8_t return_code = 0;

    // Implementation for inserting key-value pair
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key, dict->key_size);

    struct dictionary_entry* new_entry = (struct dictionary_entry*)calloc(1, sizeof(struct dictionary_entry));
    if (dict->copy_type == DICTIONARY_SHALLOW_COPY)
    {
        new_entry->key = (void*)key;
        new_entry->value = (void*)value;
    }
    else
    {
        new_entry->key = calloc(1, dict->key_size);
        if (new_entry->key == NULL) 
            return_code = 2;
        else 
            dict->key_copy_func(key, new_entry->key);
        
        new_entry->value = calloc(1, dict->value_size);
        if (new_entry->value == NULL) 
            return_code = 2;
        else 
            dict->value_copy_func(value, new_entry->value);
    }

    uint8_t added_entry = 0;

    uint64_t min_entry_stack = 0;
    uint16_t min_entry_stack_array = 0;
    uint64_t min_entry_stack_index = 0;

    if (dict->key_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM)
    {
        const uint64_t custom_key_size = dict->key_size;
        
        for (int i = 0; i < dict->array_count; i++)
        {
            const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
            const uint64_t entry_index = i * dict->array_size + index;

            // Count entries in this bucket
            uint64_t entry_stack = 0;
            struct dictionary_entry* entry = dict->entries[entry_index];
            while (entry != NULL)
            {
                if (__custom_compare__(entry->key, key, custom_key_size) == 0)
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
    }
    else
    {
        const comparator_func key_compare_func = __get_dictionary_key_compare_function__(dict->key_type);

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
    }

    if (added_entry == 0 && return_code == 0)
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

    if (return_code == 0) return return_code;
    // Error occurred; was not inserted, need to delete alocated memory

    free(new_entry);
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
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key, dict->key_size);

    uint8_t key_found = 0;
    uint8_t return_code = 2;
    
    int i = 0;
    if (dict->key_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM)
    {
        const uint64_t custom_key_size = dict->key_size;

        for (; i < dict->array_count; i++)
        {
            const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
            const uint64_t entry_index = i * dict->array_size + index;
    
            struct dictionary_entry* entry = dict->entries[entry_index];
            while (entry != NULL)
            {
                if (__custom_compare__(entry->key, key, custom_key_size) == 0)
                {
                    key_found = 1;
    
                    if (dict->copy_type == DICTIONARY_SHALLOW_COPY)
                    {
                        entry->value = (void*)value;
                    }
                    else
                    {
                        if (entry->value == NULL) 
                            return_code = 2;
                        else 
                            dict->value_copy_func(value, entry->value);
                    }
    
                    return_code = 0;
                    break;
                }
                entry = entry->next_in_bucket;
            }
        }
    }
    else
    {
        const comparator_func key_compare_func = __get_dictionary_key_compare_function__(dict->key_type);

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
    
                    if (dict->copy_type == DICTIONARY_SHALLOW_COPY)
                    {
                        entry->value = (void*)value;
                    }
                    else
                    {
                        if (entry->value == NULL) 
                            return_code = 2;
                        else 
                            dict->value_copy_func(value, entry->value);
                    }
    
                    return_code = 0;
                    break;
                }
                entry = entry->next_in_bucket;
            }
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
 */
static inline void delete_key_value_pair_dictionary(Dictionary* const dict, const void* const key)
{
    // Implementation for deleting key-value pair by key
    // Note: This is a simplified version and does not handle all edge cases
    dyn_array* const key_array = __dictionary_key_to_dyn_array__(dict->key_type, key, dict->key_size);

    if (dict->key_type == DICTIONARY_KEY_VALUE_TYPE_CUSTOM)
    {
        const uint64_t custom_key_size = dict->key_size;

        for (int i = 0; i < dict->array_count; i++)
        {
            const uint64_t index = compute_index_in_dictionary(dict->hash_function, dict->array_size, dict->hash_seeds[i], key_array);
            const uint64_t entry_index = i * dict->array_size + index;
    
            struct dictionary_entry* entry = dict->entries[entry_index];
            while (entry != NULL)
            {
                if (__custom_compare__(entry->key, key, custom_key_size) == 0)
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
                    
                    if (dict->copy_type == DICTIONARY_DEEP_COPY)
                    {
                        dict->key_cleanup_func(entry->key);
                        free(entry->key);
                        dict->value_cleanup_func(entry->value);
                        free(entry->value);
                    }
                    free(entry);
                    free_dyn_array(key_array);
                    return;
                }
                entry = entry->next_in_bucket;
            }
        }
    }
    else
    {
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
    
                    if (dict->copy_type == DICTIONARY_DEEP_COPY)
                    {
                        dict->key_cleanup_func(entry->key);
                        free(entry->key);
                        dict->value_cleanup_func(entry->value);
                        free(entry->value);
                    }
                    free(entry);
                    free_dyn_array(key_array);
                    return;
                }
                entry = entry->next_in_bucket;
            }
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
            if (dict->copy_type == DICTIONARY_DEEP_COPY)
            {
                dict->key_cleanup_func(entry->key);
                free(entry->key);
                dict->value_cleanup_func(entry->value);
                free(entry->value);
            }

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
#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include "../Vectors/vector_standards.h"
#include "../Vectors/vector2.h"
#include "../Vectors/vector3.h"
#include "../Vectors/vector4.h"
#include "../Vectors/matrix_2x2.h"
#include "../Vectors/matrix_3x3.h"
#include "../Vectors/matrix_4x4.h"
#include "../Strings/String.h"

#include <stdlib.h>
#include <stdint.h>

enum dyn_array_type {
	DYN_ARRAY_NO_TYPE,
	DYN_ARRAY_CHAR_TYPE,
	DYN_ARRAY_INT_TYPE,
	DYN_ARRAY_UINT_TYPE,
	DYN_ARRAY_UINT_8T_TYPE,
	DYN_ARRAY_UINT_16T_TYPE,
	DYN_ARRAY_UINT_32T_TYPE,
	DYN_ARRAY_UINT_64T_TYPE,
	DYN_ARRAY_VECTOR_2_TYPE,
	DYN_ARRAY_VECTOR_3_TYPE,
	DYN_ARRAY_VECTOR_4_TYPE,
	DYN_ARRAY_MATRIX_2X2_TYPE,
	DYN_ARRAY_MATRIX_3X3_TYPE,
	DYN_ARRAY_MATRIX_4X4_TYPE,
	DYN_ARRAY_STRING_TYPE
};

enum dyn_array_expansion_type {
	DYN_ARRAY_EXPANSION_DOUBLE,
	DYN_ARRAY_EXPANSION_FIXED
};

typedef struct dyn_array {
	enum dyn_array_type type;
	enum dyn_array_expansion_type expansion_type;
	unsigned int current_size;
	unsigned int max_size;
	unsigned int item_size; // bytes
	void* data;
} dyn_array;

#define dyn_get_void_ptr(DYN_ARRAY_STRUCT_PTR, INDEX) ((DYN_ARRAY_STRUCT_PTR)->data + (INDEX)*((DYN_ARRAY_STRUCT_PTR)->item_size))
#define dyn_get_last_void_ptr(DYN_ARRAY_STRUCT_PTR) dyn_get_void_ptr((DYN_ARRAY_STRUCT_PTR), (DYN_ARRAY_STRUCT_PTR)->current_size - 1)

#define dyn_get_char(DYN_ARRAY_DATA, INDEX) ((char*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_int(DYN_ARRAY_DATA, INDEX) ((int*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_uint(DYN_ARRAY_DATA, INDEX) ((unsigned int*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_uint_8t(DYN_ARRAY_DATA, INDEX) ((uint8_t*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_uint_16t(DYN_ARRAY_DATA, INDEX) ((uint16_t*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_uint_32t(DYN_ARRAY_DATA, INDEX) ((uint32_t*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_uint_64t(DYN_ARRAY_DATA, INDEX) ((uint64_t*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec2(DYN_ARRAY_DATA, INDEX) ((vector2*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec3(DYN_ARRAY_DATA, INDEX) ((vector3*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec4(DYN_ARRAY_DATA, INDEX) ((vector4*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_2x2(DYN_ARRAY_DATA, INDEX) ((matrix_2x2*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_3x3(DYN_ARRAY_DATA, INDEX) ((matrix_3x3*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_4x4(DYN_ARRAY_DATA, INDEX) ((matrix_4x4*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_string(DYN_ARRAY_DATA, INDEX) ((String*)DYN_ARRAY_DATA)[INDEX]

#define dyn_get_last_char(DYN_ARRAY_STRUCT_PTR) dyn_get_char((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_int(DYN_ARRAY_STRUCT_PTR) dyn_get_int((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_uint(DYN_ARRAY_STRUCT_PTR) dyn_get_uint((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_uint_8t(DYN_ARRAY_STRUCT_PTR) dyn_get_uint_8t((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_uint_16t(DYN_ARRAY_STRUCT_PTR) dyn_get_uint_16t((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_uint_32t(DYN_ARRAY_STRUCT_PTR) dyn_get_uint_32t((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_uint_64t(DYN_ARRAY_STRUCT_PTR) dyn_get_uint_64t((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec2(DYN_ARRAY_STRUCT_PTR) dyn_get_vec2((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec3(DYN_ARRAY_STRUCT_PTR) dyn_get_vec3((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec4(DYN_ARRAY_STRUCT_PTR) dyn_get_vec4((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_2x2(DYN_ARRAY_STRUCT_PTR) dyn_get_2x2((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_3x3(DYN_ARRAY_STRUCT_PTR) dyn_get_3x3((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_4x4(DYN_ARRAY_STRUCT_PTR) dyn_get_4x4((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_string(DYN_ARRAY_STRUCT_PTR) dyn_get_string((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)


static inline void set_dyn_array(dyn_array* const dyn_struct, const enum dyn_array_type type, const enum dyn_array_expansion_type expansion_type)
{
	dyn_struct->type = type;
	dyn_struct->expansion_type = expansion_type;
	dyn_struct->current_size = 0;
	dyn_struct->max_size = 0;
	dyn_struct->data = NULL;

	switch (dyn_struct->type)
	{
		case DYN_ARRAY_CHAR_TYPE:
			dyn_struct->item_size = sizeof(char);
			break;
		case DYN_ARRAY_INT_TYPE:
			dyn_struct->item_size = sizeof(int);
			break;
		case DYN_ARRAY_UINT_TYPE:
			dyn_struct->item_size = sizeof(unsigned int);
			break;
		case DYN_ARRAY_UINT_8T_TYPE:
			dyn_struct->item_size = sizeof(uint8_t);
			break;
		case DYN_ARRAY_UINT_16T_TYPE:
			dyn_struct->item_size = sizeof(uint16_t);
			break;
		case DYN_ARRAY_UINT_32T_TYPE:
			dyn_struct->item_size = sizeof(uint32_t);
			break;
		case DYN_ARRAY_UINT_64T_TYPE:
			dyn_struct->item_size = sizeof(uint64_t);
			break;
		case DYN_ARRAY_VECTOR_2_TYPE:
			dyn_struct->item_size = sizeof(vector2);
			break;
		case DYN_ARRAY_VECTOR_3_TYPE:
			dyn_struct->item_size = sizeof(vector3);
			break;
		case DYN_ARRAY_VECTOR_4_TYPE:
			dyn_struct->item_size = sizeof(vector4);
			break;
		case DYN_ARRAY_MATRIX_2X2_TYPE:
			dyn_struct->item_size = sizeof(matrix_2x2);
			break;
		case DYN_ARRAY_MATRIX_3X3_TYPE:
			dyn_struct->item_size = sizeof(matrix_3x3);
			break;
		case DYN_ARRAY_MATRIX_4X4_TYPE:
			dyn_struct->item_size = sizeof(matrix_4x4);
			break;
		case DYN_ARRAY_STRING_TYPE:
			dyn_struct->item_size = sizeof(String);
			break;
		default:
			dyn_struct->item_size = 1;
			break;
	}
}

static inline struct dyn_array* new_dyn_array(const enum dyn_array_type type, const enum dyn_array_expansion_type expansion_type)
{
	dyn_array* const dyn_struct = (dyn_array*)calloc(1, sizeof(dyn_array));
	set_dyn_array(dyn_struct, type, expansion_type);
	return dyn_struct;
}

/*
	To be used for custom data types; only once BEFORE adding any items
*/
static inline void override_item_size_dyn_array(dyn_array* const dyn_struct, const unsigned int size)
{
	dyn_struct->item_size = size;
}

static inline void* get_dyn_array(const dyn_array* const dyn_struct, const unsigned int index)
{
	if (dyn_struct == NULL) return NULL;
	if (index >= dyn_struct->current_size) return NULL;

	switch (dyn_struct->type)
	{
		case DYN_ARRAY_CHAR_TYPE:
			return &dyn_get_char(dyn_struct->data, index);
		case DYN_ARRAY_INT_TYPE:
			return &dyn_get_int(dyn_struct->data, index);
		case DYN_ARRAY_UINT_TYPE:
			return &dyn_get_uint(dyn_struct->data, index);
		case DYN_ARRAY_UINT_8T_TYPE:
			return &dyn_get_uint_8t(dyn_struct->data, index);
		case DYN_ARRAY_UINT_16T_TYPE:
			return &dyn_get_uint_16t(dyn_struct->data, index);
		case DYN_ARRAY_UINT_32T_TYPE:
			return &dyn_get_uint_32t(dyn_struct->data, index);
		case DYN_ARRAY_UINT_64T_TYPE:
			return &dyn_get_uint_64t(dyn_struct->data, index);
		case DYN_ARRAY_VECTOR_2_TYPE:
			return &dyn_get_vec2(dyn_struct->data, index);
		case DYN_ARRAY_VECTOR_3_TYPE:
			return &dyn_get_vec3(dyn_struct->data, index);
		case DYN_ARRAY_VECTOR_4_TYPE:
			return &dyn_get_vec4(dyn_struct->data, index);
		case DYN_ARRAY_MATRIX_2X2_TYPE:
			return &dyn_get_2x2(dyn_struct->data, index);
		case DYN_ARRAY_MATRIX_3X3_TYPE:
			return &dyn_get_3x3(dyn_struct->data, index);
		case DYN_ARRAY_MATRIX_4X4_TYPE:
			return &dyn_get_4x4(dyn_struct->data, index);
		case DYN_ARRAY_STRING_TYPE:
			return &dyn_get_string(dyn_struct->data, index);
		default:
			return ((char*)dyn_struct->data + index*(dyn_struct->item_size));
	}
}

static inline void* get_dyn_array_byte(const dyn_array* const dyn_struct, const unsigned int byte_index)
{
	if (dyn_struct == NULL) return NULL;
	if (byte_index >= dyn_struct->current_size * dyn_struct->item_size) return NULL;

	return ((char*)dyn_struct->data + byte_index);
}

static inline void* get_last_dyn_array(const dyn_array* const dyn_struct)
{
	if (dyn_struct == NULL) return NULL;
	if (dyn_struct->current_size == 0) return NULL;

	switch (dyn_struct->type)
	{
		case DYN_ARRAY_CHAR_TYPE:
			return &dyn_get_last_char(dyn_struct);
		case DYN_ARRAY_INT_TYPE:
			return &dyn_get_last_int(dyn_struct);
		case DYN_ARRAY_UINT_TYPE:
			return &dyn_get_last_uint(dyn_struct);
		case DYN_ARRAY_UINT_8T_TYPE:
			return &dyn_get_last_uint_8t(dyn_struct);
		case DYN_ARRAY_UINT_16T_TYPE:
			return &dyn_get_last_uint_16t(dyn_struct);
		case DYN_ARRAY_UINT_32T_TYPE:
			return &dyn_get_last_uint_32t(dyn_struct);
		case DYN_ARRAY_UINT_64T_TYPE:
			return &dyn_get_last_uint_64t(dyn_struct);
		case DYN_ARRAY_VECTOR_2_TYPE:
			return &dyn_get_last_vec2(dyn_struct);
		case DYN_ARRAY_VECTOR_3_TYPE:
			return &dyn_get_last_vec3(dyn_struct);
		case DYN_ARRAY_VECTOR_4_TYPE:
			return &dyn_get_last_vec4(dyn_struct);
		case DYN_ARRAY_MATRIX_2X2_TYPE:
			return &dyn_get_last_2x2(dyn_struct);
		case DYN_ARRAY_MATRIX_3X3_TYPE:
			return &dyn_get_last_3x3(dyn_struct);
		case DYN_ARRAY_MATRIX_4X4_TYPE:
			return &dyn_get_last_4x4(dyn_struct);
		case DYN_ARRAY_STRING_TYPE:
			return &dyn_get_last_string(dyn_struct);
		default:
			return ((char*)dyn_struct->data + (dyn_struct->current_size-1)*(dyn_struct->item_size));
	}
}

static inline void* add_slot_dyn_array(dyn_array* const dyn_struct)
{
	if (dyn_struct->current_size == dyn_struct->max_size)
	{
		switch (dyn_struct->expansion_type)
		{
			case DYN_ARRAY_EXPANSION_FIXED:
				dyn_struct->max_size = dyn_struct->max_size + 1;		
				break;
			case DYN_ARRAY_EXPANSION_DOUBLE:
			default:
				dyn_struct->max_size = 2 * dyn_struct->max_size + 1;		
				break;
		}
		dyn_struct->data = realloc(dyn_struct->data, dyn_struct->max_size * dyn_struct->item_size);
	}

	dyn_struct->current_size++;
	return get_last_dyn_array(dyn_struct);
}

static inline void* pop_slot_dyn_array(dyn_array* const dyn_struct, const unsigned int index)
{
	if (dyn_struct->current_size == 0) return NULL;
	if (index >= dyn_struct->current_size) return NULL;

	void* const item_ptr = get_dyn_array(dyn_struct, index);

	for (int i = index; i < dyn_struct->current_size - 1; i++)
	{
		void* const i_ptr = get_dyn_array(dyn_struct, i);
		void* const next_i_ptr = get_dyn_array(dyn_struct, i+1);
		for (int j = 0; j < dyn_struct->item_size; j++)
		{
			*((char*)i_ptr + j) = *((char*)next_i_ptr + j);
		}
	}

	dyn_struct->current_size--;
	return item_ptr;
}

static inline void append_item_dyn_array(dyn_array* const dyn_array, const void* const item)
{
	void* const slot = add_slot_dyn_array(dyn_array);
	for (int i = 0; i < dyn_array->item_size; i++)
	{
		*((char*)slot + i) = *((char*)item + i);
	}
}

static inline int strip_dyn_array(dyn_array* const dyn_struct)
{
	if (dyn_struct->current_size == 0) return -1;

	if (dyn_struct->current_size < dyn_struct->max_size)
	{
		dyn_struct->max_size = dyn_struct->current_size;
		dyn_struct->data = realloc(dyn_struct->data, dyn_struct->max_size * dyn_struct->item_size);
	}

	return dyn_struct->max_size;
}

static inline void clean_dyn_array(dyn_array* const dyn_struct)
{
	if (dyn_struct->data != NULL) free(dyn_struct->data);
	dyn_struct->data = NULL;
	dyn_struct->current_size = 0;
	dyn_struct->max_size = 0;
}

static inline void free_dyn_array(dyn_array* const dyn_struct)
{
	clean_dyn_array(dyn_struct);
	free(dyn_struct);
}

#endif
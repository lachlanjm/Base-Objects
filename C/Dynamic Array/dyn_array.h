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

enum dyn_array_type {
	DYN_ARRAY_NO_TYPE,
	DYN_ARRAY_INT_TYPE,
	DYN_ARRAY_UINT_TYPE,
	DYN_ARRAY_VECTOR_2_TYPE,
	DYN_ARRAY_VECTOR_3_TYPE,
	DYN_ARRAY_VECTOR_4_TYPE,
	DYN_ARRAY_MATRIX_2X2_TYPE,
	DYN_ARRAY_MATRIX_3X3_TYPE,
	DYN_ARRAY_MATRIX_4X4_TYPE,
	DYN_ARRAY_STRING_TYPE
};

typedef struct dyn_array {
	enum dyn_array_type type;
	unsigned int current_size;
	unsigned int max_size;
	unsigned int item_size; // bytes
	void* data;
} dyn_array;

#define dyn_get_void_ptr(DYN_ARRAY_STRUCT_PTR, INDEX) ((DYN_ARRAY_STRUCT_PTR)->data + (INDEX)*((DYN_ARRAY_STRUCT_PTR)->item_size))
#define dyn_get_last_void_ptr(DYN_ARRAY_STRUCT_PTR) dyn_get_void_ptr((DYN_ARRAY_STRUCT_PTR), (DYN_ARRAY_STRUCT_PTR)->current_size - 1)

#define dyn_get_int(DYN_ARRAY_DATA, INDEX) ((int*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_uint(DYN_ARRAY_DATA, INDEX) ((unsigned int*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec2(DYN_ARRAY_DATA, INDEX) ((vector2*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec3(DYN_ARRAY_DATA, INDEX) ((vector3*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_vec4(DYN_ARRAY_DATA, INDEX) ((vector4*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_2x2(DYN_ARRAY_DATA, INDEX) ((matrix_2x2*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_3x3(DYN_ARRAY_DATA, INDEX) ((matrix_3x3*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_4x4(DYN_ARRAY_DATA, INDEX) ((matrix_4x4*)DYN_ARRAY_DATA)[INDEX]
#define dyn_get_string(DYN_ARRAY_DATA, INDEX) ((String*)DYN_ARRAY_DATA)[INDEX]

#define dyn_get_last_int(DYN_ARRAY_STRUCT_PTR) dyn_get_int((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_uint(DYN_ARRAY_STRUCT_PTR) dyn_get_uint((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec2(DYN_ARRAY_STRUCT_PTR) dyn_get_vec2((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec3(DYN_ARRAY_STRUCT_PTR) dyn_get_vec3((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_vec4(DYN_ARRAY_STRUCT_PTR) dyn_get_vec4((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_2x2(DYN_ARRAY_STRUCT_PTR) dyn_get_2x2((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_3x3(DYN_ARRAY_STRUCT_PTR) dyn_get_3x3((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_4x4(DYN_ARRAY_STRUCT_PTR) dyn_get_4x4((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)
#define dyn_get_last_string(DYN_ARRAY_STRUCT_PTR) dyn_get_string((DYN_ARRAY_STRUCT_PTR)->data, (DYN_ARRAY_STRUCT_PTR)->current_size - 1)

static inline void set_dyn_array(dyn_array* const dyn_struct, const enum dyn_array_type type)
{
	dyn_struct->type = type;
	dyn_struct->current_size = 0;
	dyn_struct->max_size = 0;
	dyn_struct->data = NULL;

	switch (dyn_struct->type)
	{
		case DYN_ARRAY_INT_TYPE:
			dyn_struct->item_size = sizeof(int);
			break;
		case DYN_ARRAY_UINT_TYPE:
			dyn_struct->item_size = sizeof(unsigned int);
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
		case DYN_ARRAY_INT_TYPE:
			return &dyn_get_int(dyn_struct->data, index);
		case DYN_ARRAY_UINT_TYPE:
			return &dyn_get_uint(dyn_struct->data, index);
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
			return ((int*)dyn_struct->data + index*(dyn_struct->item_size));
	}
}

static inline void* get_last_dyn_array(const dyn_array* const dyn_struct)
{
	if (dyn_struct == NULL) return NULL;
	if (dyn_struct->current_size == 0) return NULL;

	switch (dyn_struct->type)
	{
		case DYN_ARRAY_INT_TYPE:
			return &dyn_get_last_int(dyn_struct);
		case DYN_ARRAY_UINT_TYPE:
			return &dyn_get_last_uint(dyn_struct);
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
			return ((int*)dyn_struct->data + (dyn_struct->current_size-1)*(dyn_struct->item_size));
	}
}

static inline void* add_slot_dyn_array(dyn_array* const dyn_struct)
{
	if (dyn_struct->current_size == dyn_struct->max_size)
	{
		dyn_struct->max_size = 2 * dyn_struct->max_size + 1;		
		dyn_struct->data = realloc(dyn_struct->data, dyn_struct->max_size * dyn_struct->item_size);
	}

	dyn_struct->current_size++;
	return get_last_dyn_array(dyn_struct);
}

static inline void clean_dyn_array(dyn_array* const dyn_struct)
{
	if (dyn_struct->data != NULL) free(dyn_struct->data);
}

#endif
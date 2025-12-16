#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <stdlib.h>
#include "vector_standards.h"

typedef struct vector2 
{
	VECTOR_FLT arr[2];
} vector2;

static inline void set_vec2(vector2* vector, const VECTOR_FLT x, const VECTOR_FLT y)
{
	get_vec2(vector->arr, 0) = x;
	get_vec2(vector->arr, 1) = y;
}
static inline vector2* new_vec2(const VECTOR_FLT x, const VECTOR_FLT y)
{
	vector2* vector = (vector2*) calloc(1, sizeof(vector2));
	set_vec2(vector, x, y);
	return vector;
}

static inline vector2* copy_to_vec2(const vector2* base, vector2* copy)
{
	get_vec2(copy->arr, 0) = get_vec2(base->arr, 0);
	get_vec2(copy->arr, 1) = get_vec2(base->arr, 1);
	return copy;
}

static inline vector2* normalize_vec2(vector2* vector)
{
	VECTOR_FLT mag = sqrt(get_vec2(vector->arr, 0)*get_vec2(vector->arr, 0) + get_vec2(vector->arr, 1)*get_vec2(vector->arr, 1));
	if (mag != 0)
	{
		mag = 1 / mag;
		get_vec2(vector->arr, 0) *= mag;
		get_vec2(vector->arr, 1) *= mag;
	}
	return (vector);
}

static inline VECTOR_FLT dot_vec2(const vector2* first, const vector2* second)
{
	return (get_vec2(first->arr, 0)*get_vec2(second->arr, 0) + get_vec2(first->arr, 1)*get_vec2(second->arr, 1));
}

/*
	Result stored in second
*/
static inline vector2* add_vec2s(const vector2* first, vector2* second)
{
	get_vec2(second->arr, 0) += get_vec2(first->arr, 0);
	get_vec2(second->arr, 1) += get_vec2(first->arr, 1);
	return second;
}

/*
	Result stored in second
*/
static inline vector2* sub_vec2s(const vector2* first, vector2* second)
{
	get_vec2(second->arr, 0) = get_vec2(first->arr, 0) - get_vec2(second->arr, 0);
	get_vec2(second->arr, 1) = get_vec2(first->arr, 1) - get_vec2(second->arr, 1);
	return second;
}

#endif
#ifndef MATRIX_2x2_H
#define MATRIX_2x2_H

#include "./vector_standards.h"
#include "./vector2.h"

typedef struct matrix_2x2
{
	VECTOR_FLT arr[4];
} matrix_2x2;

static inline matrix_2x2* new_mat2x2(void)
{
	return (matrix_2x2*) calloc(1, sizeof(matrix_2x2));
}

static inline void reset_mat2x2(matrix_2x2* matrix)
{
	for (int i = 0; i < 4; i++) matrix->arr[i] = 0;
}
static inline void set_identity_mat2x2(matrix_2x2* matrix)
{
	for (int i = 0; i < 4; i++) matrix->arr[i] = ((i / 2 == i % 2) ? 1 : 0);
}
static inline void set_translate_mat2x2(matrix_2x2* matrix, const VECTOR_FLT delta)
{
	get_2x2(matrix->arr, 0, 0) = 1;
	get_2x2(matrix->arr, 0, 1) = 0;

	get_2x2(matrix->arr, 1, 0) = delta;
	get_2x2(matrix->arr, 1, 1) = 1;
}
static inline void set_scale_mat2x2(matrix_2x2* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y)
{
	get_2x2(matrix->arr, 0, 0) = delta_x;
	get_2x2(matrix->arr, 0, 1) = 0;

	get_2x2(matrix->arr, 1, 0) = 0;
	get_2x2(matrix->arr, 1, 1) = delta_y;
}

/*
	CAN NOT BE THE SAME OBJ
*/
static inline matrix_2x2* transpose_to_mat2x2(const matrix_2x2* orig, matrix_2x2* transp)
{
	get_2x2(transp->arr, 0, 0) = get_2x2(orig->arr, 0, 0);
	get_2x2(transp->arr, 0, 1) = get_2x2(orig->arr, 1, 0);
	get_2x2(transp->arr, 1, 0) = get_2x2(orig->arr, 0, 1);
	get_2x2(transp->arr, 1, 1) = get_2x2(orig->arr, 1, 1);
	return transp;
}

static matrix_2x2 tmp_mat_2x2;

static inline void copy_to_tmp_buf_mat2x2(const matrix_2x2* base)
{
	get_2x2(tmp_mat_2x2.arr, 0, 0) = get_2x2(base->arr, 0, 0);
	get_2x2(tmp_mat_2x2.arr, 0, 1) = get_2x2(base->arr, 0, 1);
	get_2x2(tmp_mat_2x2.arr, 1, 0) = get_2x2(base->arr, 1, 0);
	get_2x2(tmp_mat_2x2.arr, 1, 1) = get_2x2(base->arr, 1, 1);
}

/*
	Result stored in second
*/
static inline matrix_2x2* cross_mat2x2_by_mat2x2(const matrix_2x2* first, matrix_2x2* second)
{
	copy_to_tmp_buf_mat2x2(second);

	get_2x2(second->arr, 0, 0) = get_2x2(first->arr, 0, 0) * get_2x2(tmp_mat_2x2.arr, 0, 0) + get_2x2(first->arr, 1, 0) * get_2x2(tmp_mat_2x2.arr, 0, 1);
	get_2x2(second->arr, 0, 1) = get_2x2(first->arr, 0, 1) * get_2x2(tmp_mat_2x2.arr, 0, 0) + get_2x2(first->arr, 1, 1) * get_2x2(tmp_mat_2x2.arr, 0, 1);

	get_2x2(second->arr, 1, 0) = get_2x2(first->arr, 0, 0) * get_2x2(tmp_mat_2x2.arr, 1, 0) + get_2x2(first->arr, 1, 0) * get_2x2(tmp_mat_2x2.arr, 1, 1);
	get_2x2(second->arr, 1, 1) = get_2x2(first->arr, 0, 1) * get_2x2(tmp_mat_2x2.arr, 1, 0) + get_2x2(first->arr, 1, 1) * get_2x2(tmp_mat_2x2.arr, 1, 1);

	return second;
}
/*
	Result stored in vector
*/
static inline vector2* cross_mat2x2_by_vec2(const matrix_2x2* matrix, vector2* vector)
{
	VECTOR_FLT x = get_vec3(vector->arr, 0);
	VECTOR_FLT y = get_vec3(vector->arr, 1);

	get_vec3(vector->arr, 0) = x * get_2x2(matrix->arr, 0, 0) + y * get_2x2(matrix->arr, 1, 0);
	get_vec3(vector->arr, 1) = x * get_2x2(matrix->arr, 0, 1) + y * get_2x2(matrix->arr, 1, 1);
	return vector;
}

static inline void translate_mat2x2(matrix_2x2* matrix, const VECTOR_FLT delta)
{
	set_translate_mat2x2(&tmp_mat_2x2, delta);
	cross_mat2x2_by_mat2x2(&tmp_mat_2x2, matrix);
}
static inline void scale_mat2x2(matrix_2x2* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y)
{
	set_scale_mat2x2(&tmp_mat_2x2, delta_x, delta_y);
	cross_mat2x2_by_mat2x2(&tmp_mat_2x2, matrix);
}

static inline matrix_2x2* transpose_mat2x2(matrix_2x2* transp)
{
	copy_to_tmp_buf_mat2x2(transp);
	get_2x2(transp->arr, 0, 0) = get_2x2(tmp_mat_2x2.arr, 0, 0);
	get_2x2(transp->arr, 0, 1) = get_2x2(tmp_mat_2x2.arr, 1, 0);
	get_2x2(transp->arr, 1, 0) = get_2x2(tmp_mat_2x2.arr, 0, 1);
	get_2x2(transp->arr, 1, 1) = get_2x2(tmp_mat_2x2.arr, 1, 1);
	return transp;
}

static inline matrix_2x2* copy_to_mat2x2(const matrix_2x2* base, matrix_2x2* copy)
{
	get_2x2(copy->arr, 0, 0) = get_2x2(base->arr, 0, 0);
	get_2x2(copy->arr, 0, 1) = get_2x2(base->arr, 0, 1);
	get_2x2(copy->arr, 1, 0) = get_2x2(base->arr, 1, 0);
	get_2x2(copy->arr, 1, 1) = get_2x2(base->arr, 1, 1);
	return copy;
}

// #include <stdio.h>
// static inline void dump_mat2x2(const matrix_2x2* const matrix)
// {
// 	printf("%lf, %lf\n", get_2x2(matrix->arr, 0, 0), get_2x2(matrix->arr, 1, 0));
// 	printf("%lf, %lf\n", get_2x2(matrix->arr, 0, 1), get_2x2(matrix->arr, 1, 1));
// }

#endif
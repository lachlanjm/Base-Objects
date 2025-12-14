#ifndef MATRIX_3x3_H
#define MATRIX_3x3_H

#include "./vector_standards.h"
#include "./vector3.h"

typedef struct matrix_3x3
{
	VECTOR_FLT arr[9];
} matrix_3x3;

static inline matrix_3x3* new_mat3x3(void)
{
	return (matrix_3x3*) calloc(1, sizeof(matrix_3x3));
}

static inline void reset_mat3x3(matrix_3x3* matrix)
{
	for (int i = 0; i < 9; i++) matrix->arr[i] = 0;
}
static inline void set_identity_mat3x3(matrix_3x3* matrix)
{
	for (int i = 0; i < 9; i++) matrix->arr[i] = ((i / 3 == i % 3) ? 1 : 0);
}
static inline void set_translate_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y)
{
	get_3x3(matrix->arr, 0, 0) = 1;
	get_3x3(matrix->arr, 0, 1) = 0;
	get_3x3(matrix->arr, 0, 2) = 0;

	get_3x3(matrix->arr, 1, 0) = 0;
	get_3x3(matrix->arr, 1, 1) = 1;
	get_3x3(matrix->arr, 1, 2) = 0;

	get_3x3(matrix->arr, 2, 0) = delta_x;
	get_3x3(matrix->arr, 2, 1) = delta_y;
	get_3x3(matrix->arr, 2, 2) = 1;
}
static inline void set_scale_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_3x3(matrix->arr, 0, 0) = delta_x;
	get_3x3(matrix->arr, 0, 1) = 0;
	get_3x3(matrix->arr, 0, 2) = 0;

	get_3x3(matrix->arr, 1, 0) = 0;
	get_3x3(matrix->arr, 1, 1) = delta_y;
	get_3x3(matrix->arr, 1, 2) = 0;

	get_3x3(matrix->arr, 2, 0) = 0;
	get_3x3(matrix->arr, 2, 1) = 0;
	get_3x3(matrix->arr, 2, 2) = delta_z;
}

/*
	delta_? must be in rads 
*/
static inline void set_rotate_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta)
{
	VECTOR_FLT C = cos(delta);
	VECTOR_FLT S = sin(delta);
	
	get_3x3(matrix->arr, 0, 0) = C;
	get_3x3(matrix->arr, 0, 1) = S;
	get_3x3(matrix->arr, 0, 2) = 0;

	get_3x3(matrix->arr, 1, 0) = -S;
	get_3x3(matrix->arr, 1, 1) = C;
	get_3x3(matrix->arr, 1, 2) = 0;

	get_3x3(matrix->arr, 2, 0) = 0;
	get_3x3(matrix->arr, 2, 1) = 0;
	get_3x3(matrix->arr, 2, 2) = 1;
}

/*
	CAN NOT BE THE SAME OBJ
*/
static inline matrix_3x3* transpose_to_mat3x3(const matrix_3x3* orig, matrix_3x3* transp)
{
	get_3x3(transp->arr, 0, 0) = get_3x3(orig->arr, 0, 0);
	get_3x3(transp->arr, 0, 1) = get_3x3(orig->arr, 1, 0);
	get_3x3(transp->arr, 0, 2) = get_3x3(orig->arr, 2, 0);
	get_3x3(transp->arr, 1, 0) = get_3x3(orig->arr, 0, 1);
	get_3x3(transp->arr, 1, 1) = get_3x3(orig->arr, 1, 1);
	get_3x3(transp->arr, 1, 2) = get_3x3(orig->arr, 2, 1);
	get_3x3(transp->arr, 2, 0) = get_3x3(orig->arr, 0, 2);
	get_3x3(transp->arr, 2, 1) = get_3x3(orig->arr, 1, 2);
	get_3x3(transp->arr, 2, 2) = get_3x3(orig->arr, 2, 2);
	return transp;
}

static matrix_3x3 tmp_mat_3x3;

static inline void copy_to_tmp_buf_mat3x3(const matrix_3x3* base)
{
	get_3x3(tmp_mat_3x3.arr, 0, 0) = get_3x3(base->arr, 0, 0);
	get_3x3(tmp_mat_3x3.arr, 0, 1) = get_3x3(base->arr, 0, 1);
	get_3x3(tmp_mat_3x3.arr, 0, 2) = get_3x3(base->arr, 0, 2);
	get_3x3(tmp_mat_3x3.arr, 1, 0) = get_3x3(base->arr, 1, 0);
	get_3x3(tmp_mat_3x3.arr, 1, 1) = get_3x3(base->arr, 1, 1);
	get_3x3(tmp_mat_3x3.arr, 1, 2) = get_3x3(base->arr, 1, 2);
	get_3x3(tmp_mat_3x3.arr, 2, 0) = get_3x3(base->arr, 2, 0);
	get_3x3(tmp_mat_3x3.arr, 2, 1) = get_3x3(base->arr, 2, 1);
	get_3x3(tmp_mat_3x3.arr, 2, 2) = get_3x3(base->arr, 2, 2);
}

/*
	Result stored in second
*/
static inline matrix_3x3* cross_mat3x3_by_mat3x3(const matrix_3x3* first, matrix_3x3* second)
{
	copy_to_tmp_buf_mat3x3(second);

	get_3x3(second->arr, 0, 0) = get_3x3(first->arr, 0, 0) * get_3x3(tmp_mat_3x3.arr, 0, 0) + get_3x3(first->arr, 1, 0) * get_3x3(tmp_mat_3x3.arr, 0, 1) + get_3x3(first->arr, 2, 0) * get_3x3(tmp_mat_3x3.arr, 0, 2);
	get_3x3(second->arr, 0, 1) = get_3x3(first->arr, 0, 1) * get_3x3(tmp_mat_3x3.arr, 0, 0) + get_3x3(first->arr, 1, 1) * get_3x3(tmp_mat_3x3.arr, 0, 1) + get_3x3(first->arr, 2, 1) * get_3x3(tmp_mat_3x3.arr, 0, 2);
	get_3x3(second->arr, 0, 2) = get_3x3(first->arr, 0, 2) * get_3x3(tmp_mat_3x3.arr, 0, 0) + get_3x3(first->arr, 1, 2) * get_3x3(tmp_mat_3x3.arr, 0, 1) + get_3x3(first->arr, 2, 2) * get_3x3(tmp_mat_3x3.arr, 0, 2);

	get_3x3(second->arr, 1, 0) = get_3x3(first->arr, 0, 0) * get_3x3(tmp_mat_3x3.arr, 1, 0) + get_3x3(first->arr, 1, 0) * get_3x3(tmp_mat_3x3.arr, 1, 1) + get_3x3(first->arr, 2, 0) * get_3x3(tmp_mat_3x3.arr, 1, 2);
	get_3x3(second->arr, 1, 1) = get_3x3(first->arr, 0, 1) * get_3x3(tmp_mat_3x3.arr, 1, 0) + get_3x3(first->arr, 1, 1) * get_3x3(tmp_mat_3x3.arr, 1, 1) + get_3x3(first->arr, 2, 1) * get_3x3(tmp_mat_3x3.arr, 1, 2);
	get_3x3(second->arr, 1, 2) = get_3x3(first->arr, 0, 2) * get_3x3(tmp_mat_3x3.arr, 1, 0) + get_3x3(first->arr, 1, 2) * get_3x3(tmp_mat_3x3.arr, 1, 1) + get_3x3(first->arr, 2, 2) * get_3x3(tmp_mat_3x3.arr, 1, 2);

	get_3x3(second->arr, 2, 0) = get_3x3(first->arr, 0, 0) * get_3x3(tmp_mat_3x3.arr, 2, 0) + get_3x3(first->arr, 1, 0) * get_3x3(tmp_mat_3x3.arr, 2, 1) + get_3x3(first->arr, 2, 0) * get_3x3(tmp_mat_3x3.arr, 2, 2);
	get_3x3(second->arr, 2, 1) = get_3x3(first->arr, 0, 1) * get_3x3(tmp_mat_3x3.arr, 2, 0) + get_3x3(first->arr, 1, 1) * get_3x3(tmp_mat_3x3.arr, 2, 1) + get_3x3(first->arr, 2, 1) * get_3x3(tmp_mat_3x3.arr, 2, 2);
	get_3x3(second->arr, 2, 2) = get_3x3(first->arr, 0, 2) * get_3x3(tmp_mat_3x3.arr, 2, 0) + get_3x3(first->arr, 1, 2) * get_3x3(tmp_mat_3x3.arr, 2, 1) + get_3x3(first->arr, 2, 2) * get_3x3(tmp_mat_3x3.arr, 2, 2);

	return second;
}
/*
	Result stored in vector
*/
static inline vector3* cross_mat3x3_by_vec3(const matrix_3x3* matrix, vector3* vector)
{
	VECTOR_FLT x = get_vec3(vector->arr, 0);
	VECTOR_FLT y = get_vec3(vector->arr, 1);
	VECTOR_FLT z = get_vec3(vector->arr, 2);

	get_vec3(vector->arr, 0) = x * get_3x3(matrix->arr, 0, 0) + y * get_3x3(matrix->arr, 1, 0) + z * get_3x3(matrix->arr, 2, 0);
	get_vec3(vector->arr, 1) = x * get_3x3(matrix->arr, 0, 1) + y * get_3x3(matrix->arr, 1, 1) + z * get_3x3(matrix->arr, 2, 1);
	get_vec3(vector->arr, 2) = x * get_3x3(matrix->arr, 0, 2) + y * get_3x3(matrix->arr, 1, 2) + z * get_3x3(matrix->arr, 2, 2);
	return vector;
}

static inline void translate_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y)
{
	set_translate_mat3x3(&tmp_mat_3x3, delta_x, delta_y);
	cross_mat3x3_by_mat3x3(&tmp_mat_3x3, matrix);
}
static inline void scale_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	set_scale_mat3x3(&tmp_mat_3x3, delta_x, delta_y, delta_z);
	cross_mat3x3_by_mat3x3(&tmp_mat_3x3, matrix);
}

/*
	delta_? must be in rads
*/
static inline void rotate_mat3x3(matrix_3x3* matrix, const VECTOR_FLT delta)
{
	set_rotate_mat3x3(&tmp_mat_3x3, delta);
	cross_mat3x3_by_mat3x3(&tmp_mat_3x3, matrix);
}

static inline matrix_3x3* transpose_mat3x3(matrix_3x3* transp)
{
	copy_to_tmp_buf_mat3x3(transp);
	get_3x3(transp->arr, 0, 0) = get_3x3(tmp_mat_3x3.arr, 0, 0);
	get_3x3(transp->arr, 0, 1) = get_3x3(tmp_mat_3x3.arr, 1, 0);
	get_3x3(transp->arr, 0, 2) = get_3x3(tmp_mat_3x3.arr, 2, 0);
	get_3x3(transp->arr, 1, 0) = get_3x3(tmp_mat_3x3.arr, 0, 1);
	get_3x3(transp->arr, 1, 1) = get_3x3(tmp_mat_3x3.arr, 1, 1);
	get_3x3(transp->arr, 1, 2) = get_3x3(tmp_mat_3x3.arr, 2, 1);
	get_3x3(transp->arr, 2, 0) = get_3x3(tmp_mat_3x3.arr, 0, 2);
	get_3x3(transp->arr, 2, 1) = get_3x3(tmp_mat_3x3.arr, 1, 2);
	get_3x3(transp->arr, 2, 2) = get_3x3(tmp_mat_3x3.arr, 2, 2);
	return transp;
}

static inline matrix_3x3* copy_to_mat3x3(const matrix_3x3* base, matrix_3x3* copy)
{
	get_3x3(copy->arr, 0, 0) = get_3x3(base->arr, 0, 0);
	get_3x3(copy->arr, 0, 1) = get_3x3(base->arr, 0, 1);
	get_3x3(copy->arr, 0, 2) = get_3x3(base->arr, 0, 2);
	get_3x3(copy->arr, 1, 0) = get_3x3(base->arr, 1, 0);
	get_3x3(copy->arr, 1, 1) = get_3x3(base->arr, 1, 1);
	get_3x3(copy->arr, 1, 2) = get_3x3(base->arr, 1, 2);
	get_3x3(copy->arr, 2, 0) = get_3x3(base->arr, 2, 0);
	get_3x3(copy->arr, 2, 1) = get_3x3(base->arr, 2, 1);
	get_3x3(copy->arr, 2, 2) = get_3x3(base->arr, 2, 2);
	return copy;
}

// #include <stdio.h>
// static inline void dump_mat3x3(const matrix_3x3* const matrix)
// {
// 	printf("%lf, %lf, %lf\n", get_3x3(matrix->arr, 0, 0), get_3x3(matrix->arr, 1, 0), get_3x3(matrix->arr, 2, 0));
// 	printf("%lf, %lf, %lf\n", get_3x3(matrix->arr, 0, 1), get_3x3(matrix->arr, 1, 1), get_3x3(matrix->arr, 2, 1));
// 	printf("%lf, %lf, %lf\n", get_3x3(matrix->arr, 0, 2), get_3x3(matrix->arr, 1, 2), get_3x3(matrix->arr, 2, 2));
// }

#endif
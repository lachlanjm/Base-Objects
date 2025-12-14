#ifndef MATRIX_4x4_H
#define MATRIX_4x4_H

#include "./vector_standards.h"
#include "./vector3.h"
#include "./vector4.h"

typedef struct matrix_4x4
{
	VECTOR_FLT arr[16];
} matrix_4x4;

static inline matrix_4x4* new_mat4x4(void)
{
	return (matrix_4x4*) calloc(1, sizeof(matrix_4x4));
}

static inline void reset_mat4x4(matrix_4x4* const matrix)
{
	for (int i = 0; i < 16; i++) matrix->arr[i] = 0;
}
static inline void set_identity_mat4x4(matrix_4x4* const matrix)
{
	for (int i = 0; i < 16; i++) matrix->arr[i] = ((i / 4 == i % 4) ? 1 : 0);
}
static inline void set_translate_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_4x4(matrix->arr, 0, 0) = 1;
	get_4x4(matrix->arr, 0, 1) = 0;
	get_4x4(matrix->arr, 0, 2) = 0;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = 0;
	get_4x4(matrix->arr, 1, 1) = 1;
	get_4x4(matrix->arr, 1, 2) = 0;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = 0;
	get_4x4(matrix->arr, 2, 1) = 0;
	get_4x4(matrix->arr, 2, 2) = 1;
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = delta_x;
	get_4x4(matrix->arr, 3, 1) = delta_y;
	get_4x4(matrix->arr, 3, 2) = delta_z;
	get_4x4(matrix->arr, 3, 3) = 1;
}
static inline void set_scale_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_4x4(matrix->arr, 0, 0) = delta_x;
	get_4x4(matrix->arr, 0, 1) = 0;
	get_4x4(matrix->arr, 0, 2) = 0;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = 0;
	get_4x4(matrix->arr, 1, 1) = delta_y;
	get_4x4(matrix->arr, 1, 2) = 0;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = 0;
	get_4x4(matrix->arr, 2, 1) = 0;
	get_4x4(matrix->arr, 2, 2) = delta_z;
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = 0;
	get_4x4(matrix->arr, 3, 1) = 0;
	get_4x4(matrix->arr, 3, 2) = 0;
	get_4x4(matrix->arr, 3, 3) = 1;
}

/*
	delta_? must be in rads 
*/
static inline void set_rotate_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	VECTOR_FLT A = cos(delta_x);
	VECTOR_FLT B = sin(delta_x);
	VECTOR_FLT C = cos(delta_y);
	VECTOR_FLT D = sin(delta_y);
	VECTOR_FLT E = cos(delta_z);
	VECTOR_FLT F = sin(delta_z);

	VECTOR_FLT AE = A*E;
	VECTOR_FLT AF = A*F;
	VECTOR_FLT BE = B*E;
	VECTOR_FLT BF = B*F;

	get_4x4(matrix->arr, 0, 0) = C*E;
	get_4x4(matrix->arr, 0, 1) = BE*D + AF;
	get_4x4(matrix->arr, 0, 2) = BF - AE*D;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = -C*F;
	get_4x4(matrix->arr, 1, 1) = AE - BF*D;
	get_4x4(matrix->arr, 1, 2) = AF*D + BE;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = D;
	get_4x4(matrix->arr, 2, 1) = -B*C;
	get_4x4(matrix->arr, 2, 2) = A*C;
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = 0;
	get_4x4(matrix->arr, 3, 1) = 0;
	get_4x4(matrix->arr, 3, 2) = 0;
	get_4x4(matrix->arr, 3, 3) = 1;
}

/*
	CAN NOT BE THE SAME OBJ
*/
static inline matrix_4x4* transpose_to_mat4x4(const matrix_4x4* const orig, matrix_4x4* const transp)
{
	get_4x4(transp->arr, 0, 0) = get_4x4(orig->arr, 0, 0);
	get_4x4(transp->arr, 0, 1) = get_4x4(orig->arr, 1, 0);
	get_4x4(transp->arr, 0, 2) = get_4x4(orig->arr, 2, 0);
	get_4x4(transp->arr, 0, 3) = get_4x4(orig->arr, 3, 0);
	get_4x4(transp->arr, 1, 0) = get_4x4(orig->arr, 0, 1);
	get_4x4(transp->arr, 1, 1) = get_4x4(orig->arr, 1, 1);
	get_4x4(transp->arr, 1, 2) = get_4x4(orig->arr, 2, 1);
	get_4x4(transp->arr, 1, 3) = get_4x4(orig->arr, 3, 1);
	get_4x4(transp->arr, 2, 0) = get_4x4(orig->arr, 0, 2);
	get_4x4(transp->arr, 2, 1) = get_4x4(orig->arr, 1, 2);
	get_4x4(transp->arr, 2, 2) = get_4x4(orig->arr, 2, 2);
	get_4x4(transp->arr, 2, 3) = get_4x4(orig->arr, 3, 2);
	get_4x4(transp->arr, 3, 0) = get_4x4(orig->arr, 0, 3);
	get_4x4(transp->arr, 3, 1) = get_4x4(orig->arr, 1, 3);
	get_4x4(transp->arr, 3, 2) = get_4x4(orig->arr, 2, 3);
	get_4x4(transp->arr, 3, 3) = get_4x4(orig->arr, 3, 3);
	return transp;
}

static matrix_4x4 tmp_mat_4x4;

static inline void copy_to_tmp_buf_mat4x4(const matrix_4x4* const base)
{
	get_4x4(tmp_mat_4x4.arr, 0, 0) = get_4x4(base->arr, 0, 0);
	get_4x4(tmp_mat_4x4.arr, 0, 1) = get_4x4(base->arr, 0, 1);
	get_4x4(tmp_mat_4x4.arr, 0, 2) = get_4x4(base->arr, 0, 2);
	get_4x4(tmp_mat_4x4.arr, 0, 3) = get_4x4(base->arr, 0, 3);
	get_4x4(tmp_mat_4x4.arr, 1, 0) = get_4x4(base->arr, 1, 0);
	get_4x4(tmp_mat_4x4.arr, 1, 1) = get_4x4(base->arr, 1, 1);
	get_4x4(tmp_mat_4x4.arr, 1, 2) = get_4x4(base->arr, 1, 2);
	get_4x4(tmp_mat_4x4.arr, 1, 3) = get_4x4(base->arr, 1, 3);
	get_4x4(tmp_mat_4x4.arr, 2, 0) = get_4x4(base->arr, 2, 0);
	get_4x4(tmp_mat_4x4.arr, 2, 1) = get_4x4(base->arr, 2, 1);
	get_4x4(tmp_mat_4x4.arr, 2, 2) = get_4x4(base->arr, 2, 2);
	get_4x4(tmp_mat_4x4.arr, 2, 3) = get_4x4(base->arr, 2, 3);
	get_4x4(tmp_mat_4x4.arr, 3, 0) = get_4x4(base->arr, 3, 0);
	get_4x4(tmp_mat_4x4.arr, 3, 1) = get_4x4(base->arr, 3, 1);
	get_4x4(tmp_mat_4x4.arr, 3, 2) = get_4x4(base->arr, 3, 2);
	get_4x4(tmp_mat_4x4.arr, 3, 3) = get_4x4(base->arr, 3, 3);
}

/*
	Result stored in second
*/
static inline matrix_4x4* cross_mat4x4_by_mat4x4(const matrix_4x4* const first, matrix_4x4* const second)
{
	copy_to_tmp_buf_mat4x4(second);

	get_4x4(second->arr, 0, 0) = get_4x4(first->arr, 0, 0) * get_4x4(tmp_mat_4x4.arr, 0, 0) + get_4x4(first->arr, 1, 0) * get_4x4(tmp_mat_4x4.arr, 0, 1) + get_4x4(first->arr, 2, 0) * get_4x4(tmp_mat_4x4.arr, 0, 2) + get_4x4(first->arr, 3, 0) * get_4x4(tmp_mat_4x4.arr, 0, 3);
	get_4x4(second->arr, 0, 1) = get_4x4(first->arr, 0, 1) * get_4x4(tmp_mat_4x4.arr, 0, 0) + get_4x4(first->arr, 1, 1) * get_4x4(tmp_mat_4x4.arr, 0, 1) + get_4x4(first->arr, 2, 1) * get_4x4(tmp_mat_4x4.arr, 0, 2) + get_4x4(first->arr, 3, 1) * get_4x4(tmp_mat_4x4.arr, 0, 3);
	get_4x4(second->arr, 0, 2) = get_4x4(first->arr, 0, 2) * get_4x4(tmp_mat_4x4.arr, 0, 0) + get_4x4(first->arr, 1, 2) * get_4x4(tmp_mat_4x4.arr, 0, 1) + get_4x4(first->arr, 2, 2) * get_4x4(tmp_mat_4x4.arr, 0, 2) + get_4x4(first->arr, 3, 2) * get_4x4(tmp_mat_4x4.arr, 0, 3);
	get_4x4(second->arr, 0, 3) = get_4x4(first->arr, 0, 3) * get_4x4(tmp_mat_4x4.arr, 0, 0) + get_4x4(first->arr, 1, 3) * get_4x4(tmp_mat_4x4.arr, 0, 1) + get_4x4(first->arr, 2, 3) * get_4x4(tmp_mat_4x4.arr, 0, 2) + get_4x4(first->arr, 3, 3) * get_4x4(tmp_mat_4x4.arr, 0, 3);

	get_4x4(second->arr, 1, 0) = get_4x4(first->arr, 0, 0) * get_4x4(tmp_mat_4x4.arr, 1, 0) + get_4x4(first->arr, 1, 0) * get_4x4(tmp_mat_4x4.arr, 1, 1) + get_4x4(first->arr, 2, 0) * get_4x4(tmp_mat_4x4.arr, 1, 2) + get_4x4(first->arr, 3, 0) * get_4x4(tmp_mat_4x4.arr, 1, 3);
	get_4x4(second->arr, 1, 1) = get_4x4(first->arr, 0, 1) * get_4x4(tmp_mat_4x4.arr, 1, 0) + get_4x4(first->arr, 1, 1) * get_4x4(tmp_mat_4x4.arr, 1, 1) + get_4x4(first->arr, 2, 1) * get_4x4(tmp_mat_4x4.arr, 1, 2) + get_4x4(first->arr, 3, 1) * get_4x4(tmp_mat_4x4.arr, 1, 3);
	get_4x4(second->arr, 1, 2) = get_4x4(first->arr, 0, 2) * get_4x4(tmp_mat_4x4.arr, 1, 0) + get_4x4(first->arr, 1, 2) * get_4x4(tmp_mat_4x4.arr, 1, 1) + get_4x4(first->arr, 2, 2) * get_4x4(tmp_mat_4x4.arr, 1, 2) + get_4x4(first->arr, 3, 2) * get_4x4(tmp_mat_4x4.arr, 1, 3);
	get_4x4(second->arr, 1, 3) = get_4x4(first->arr, 0, 3) * get_4x4(tmp_mat_4x4.arr, 1, 0) + get_4x4(first->arr, 1, 3) * get_4x4(tmp_mat_4x4.arr, 1, 1) + get_4x4(first->arr, 2, 3) * get_4x4(tmp_mat_4x4.arr, 1, 2) + get_4x4(first->arr, 3, 3) * get_4x4(tmp_mat_4x4.arr, 1, 3);

	get_4x4(second->arr, 2, 0) = get_4x4(first->arr, 0, 0) * get_4x4(tmp_mat_4x4.arr, 2, 0) + get_4x4(first->arr, 1, 0) * get_4x4(tmp_mat_4x4.arr, 2, 1) + get_4x4(first->arr, 2, 0) * get_4x4(tmp_mat_4x4.arr, 2, 2) + get_4x4(first->arr, 3, 0) * get_4x4(tmp_mat_4x4.arr, 2, 3);
	get_4x4(second->arr, 2, 1) = get_4x4(first->arr, 0, 1) * get_4x4(tmp_mat_4x4.arr, 2, 0) + get_4x4(first->arr, 1, 1) * get_4x4(tmp_mat_4x4.arr, 2, 1) + get_4x4(first->arr, 2, 1) * get_4x4(tmp_mat_4x4.arr, 2, 2) + get_4x4(first->arr, 3, 1) * get_4x4(tmp_mat_4x4.arr, 2, 3);
	get_4x4(second->arr, 2, 2) = get_4x4(first->arr, 0, 2) * get_4x4(tmp_mat_4x4.arr, 2, 0) + get_4x4(first->arr, 1, 2) * get_4x4(tmp_mat_4x4.arr, 2, 1) + get_4x4(first->arr, 2, 2) * get_4x4(tmp_mat_4x4.arr, 2, 2) + get_4x4(first->arr, 3, 2) * get_4x4(tmp_mat_4x4.arr, 2, 3);
	get_4x4(second->arr, 2, 3) = get_4x4(first->arr, 0, 3) * get_4x4(tmp_mat_4x4.arr, 2, 0) + get_4x4(first->arr, 1, 3) * get_4x4(tmp_mat_4x4.arr, 2, 1) + get_4x4(first->arr, 2, 3) * get_4x4(tmp_mat_4x4.arr, 2, 2) + get_4x4(first->arr, 3, 3) * get_4x4(tmp_mat_4x4.arr, 2, 3);

	get_4x4(second->arr, 3, 0) = get_4x4(first->arr, 0, 0) * get_4x4(tmp_mat_4x4.arr, 3, 0) + get_4x4(first->arr, 1, 0) * get_4x4(tmp_mat_4x4.arr, 3, 1) + get_4x4(first->arr, 2, 0) * get_4x4(tmp_mat_4x4.arr, 3, 2) + get_4x4(first->arr, 3, 0) * get_4x4(tmp_mat_4x4.arr, 3, 3);
	get_4x4(second->arr, 3, 1) = get_4x4(first->arr, 0, 1) * get_4x4(tmp_mat_4x4.arr, 3, 0) + get_4x4(first->arr, 1, 1) * get_4x4(tmp_mat_4x4.arr, 3, 1) + get_4x4(first->arr, 2, 1) * get_4x4(tmp_mat_4x4.arr, 3, 2) + get_4x4(first->arr, 3, 1) * get_4x4(tmp_mat_4x4.arr, 3, 3);
	get_4x4(second->arr, 3, 2) = get_4x4(first->arr, 0, 2) * get_4x4(tmp_mat_4x4.arr, 3, 0) + get_4x4(first->arr, 1, 2) * get_4x4(tmp_mat_4x4.arr, 3, 1) + get_4x4(first->arr, 2, 2) * get_4x4(tmp_mat_4x4.arr, 3, 2) + get_4x4(first->arr, 3, 2) * get_4x4(tmp_mat_4x4.arr, 3, 3);
	get_4x4(second->arr, 3, 3) = get_4x4(first->arr, 0, 3) * get_4x4(tmp_mat_4x4.arr, 3, 0) + get_4x4(first->arr, 1, 3) * get_4x4(tmp_mat_4x4.arr, 3, 1) + get_4x4(first->arr, 2, 3) * get_4x4(tmp_mat_4x4.arr, 3, 2) + get_4x4(first->arr, 3, 3) * get_4x4(tmp_mat_4x4.arr, 3, 3);

	return second;
}
/*
	Result stored in vector
*/
static inline vector4* cross_mat4x4_by_vec4(const matrix_4x4* const matrix, vector4* const vector)
{
	VECTOR_FLT x = get_vec4(vector->arr, 0);
	VECTOR_FLT y = get_vec4(vector->arr, 1);
	VECTOR_FLT z = get_vec4(vector->arr, 2);
	VECTOR_FLT w = get_vec4(vector->arr, 3);

	get_vec4(vector->arr, 0) = x * get_4x4(matrix->arr, 0, 0) + y * get_4x4(matrix->arr, 1, 0) + z * get_4x4(matrix->arr, 2, 0) + w * get_4x4(matrix->arr, 3, 0);
	get_vec4(vector->arr, 1) = x * get_4x4(matrix->arr, 0, 1) + y * get_4x4(matrix->arr, 1, 1) + z * get_4x4(matrix->arr, 2, 1) + w * get_4x4(matrix->arr, 3, 1);
	get_vec4(vector->arr, 2) = x * get_4x4(matrix->arr, 0, 2) + y * get_4x4(matrix->arr, 1, 2) + z * get_4x4(matrix->arr, 2, 2) + w * get_4x4(matrix->arr, 3, 2);
	get_vec4(vector->arr, 3) = x * get_4x4(matrix->arr, 0, 3) + y * get_4x4(matrix->arr, 1, 3) + z * get_4x4(matrix->arr, 2, 3) + w * get_4x4(matrix->arr, 3, 3);
	return vector;
}

static inline void translate_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	set_translate_mat4x4(&tmp_mat_4x4, delta_x, delta_y, delta_z);
	cross_mat4x4_by_mat4x4(&tmp_mat_4x4, matrix);
}
static inline void scale_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	set_scale_mat4x4(&tmp_mat_4x4, delta_x, delta_y, delta_z);
	cross_mat4x4_by_mat4x4(&tmp_mat_4x4, matrix);
}

/*
	delta_? must be in rads
*/
static inline void rotate_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	set_rotate_mat4x4(&tmp_mat_4x4, delta_x, delta_y, delta_z);
	cross_mat4x4_by_mat4x4(&tmp_mat_4x4, matrix);
}

static inline void set_perspective_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT fov, const VECTOR_FLT aspect, const VECTOR_FLT near, const VECTOR_FLT far)
{
	VECTOR_FLT tan_fov = tan((VECTOR_FLT)fov/2.0f);
	if (tan_fov == 0)
	{
		get_4x4(matrix->arr, 0, 0) = 0;
		get_4x4(matrix->arr, 1, 1) = 0; // idk???
	}
	else
	{
		get_4x4(matrix->arr, 0, 0) = 1 / (aspect * tan_fov);
		get_4x4(matrix->arr, 1, 1) = 1 / tan_fov;
	}
	
	// get_4x4(matrix->arr, 0, 0) ^^
	get_4x4(matrix->arr, 0, 1) = 0;
	get_4x4(matrix->arr, 0, 2) = 0;
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = 0;
	// get_4x4(matrix->arr, 1, 1) ^^
	get_4x4(matrix->arr, 1, 2) = 0;
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = 0;
	get_4x4(matrix->arr, 2, 1) = 0;
	get_4x4(matrix->arr, 2, 2) = (VECTOR_FLT)(far + near) / (VECTOR_FLT)(near - far);
	get_4x4(matrix->arr, 2, 3) = -1;

	get_4x4(matrix->arr, 3, 0) = 0;
	get_4x4(matrix->arr, 3, 1) = 0;
	get_4x4(matrix->arr, 3, 2) = (VECTOR_FLT)(2*far*near) / (VECTOR_FLT)(near - far);
	get_4x4(matrix->arr, 3, 3) = 0;
}
static inline void set_look_at_mat4x4(matrix_4x4* const matrix, const vector3* location, const vector3* fixation, const vector3* rotation)
{
	vector3 x_axis;
	vector3 y_axis;
	vector3 z_axis;

	normalize_vec3(sub_vec3s(fixation, copy_to_vec3(location, &z_axis))); // Sets z-axis
	normalize_vec3(cross_vec3_by_vec3(&z_axis, copy_to_vec3(rotation, &x_axis))); // Sets x-axis
	cross_vec3_by_vec3(&x_axis, copy_to_vec3(&z_axis, &y_axis)); // Sets y-axis

	get_4x4(matrix->arr, 0, 0) = get_vec3(x_axis.arr, 0);
	get_4x4(matrix->arr, 0, 1) = get_vec3(y_axis.arr, 0);
	get_4x4(matrix->arr, 0, 2) = -get_vec3(z_axis.arr, 0);
	get_4x4(matrix->arr, 0, 3) = 0;

	get_4x4(matrix->arr, 1, 0) = get_vec3(x_axis.arr, 1);
	get_4x4(matrix->arr, 1, 1) = get_vec3(y_axis.arr, 1);
	get_4x4(matrix->arr, 1, 2) = -get_vec3(z_axis.arr, 1);
	get_4x4(matrix->arr, 1, 3) = 0;

	get_4x4(matrix->arr, 2, 0) = get_vec3(x_axis.arr, 2);
	get_4x4(matrix->arr, 2, 1) = get_vec3(y_axis.arr, 2);
	get_4x4(matrix->arr, 2, 2) = -get_vec3(z_axis.arr, 2);
	get_4x4(matrix->arr, 2, 3) = 0;

	get_4x4(matrix->arr, 3, 0) = -dot_vec3(&x_axis, location);
	get_4x4(matrix->arr, 3, 1) = -dot_vec3(&y_axis, location);
	get_4x4(matrix->arr, 3, 2) = dot_vec3(&z_axis, location);
	get_4x4(matrix->arr, 3, 3) = 1;
}

static inline void transform_perspective_mat4x4(matrix_4x4* const matrix, const VECTOR_FLT fov, const VECTOR_FLT aspect, const VECTOR_FLT _near, const VECTOR_FLT _far)
{
	matrix_4x4 tmp_infunc_mat;
	set_perspective_mat4x4(&tmp_infunc_mat, fov, aspect, _near, _far);
	cross_mat4x4_by_mat4x4(&tmp_infunc_mat, matrix);
}
static inline void transform_look_at_mat4x4(matrix_4x4* const matrix, const vector3* location, const vector3* fixation, const vector3* rotation)
{
	matrix_4x4 tmp_infunc_mat;
	set_look_at_mat4x4(&tmp_infunc_mat, location, fixation, rotation);
	cross_mat4x4_by_mat4x4(&tmp_infunc_mat, matrix);
}

static inline matrix_4x4* transpose_mat4x4(matrix_4x4* const transp)
{
	copy_to_tmp_buf_mat4x4(transp);
	get_4x4(transp->arr, 0, 0) = get_4x4(tmp_mat_4x4.arr, 0, 0);
	get_4x4(transp->arr, 0, 1) = get_4x4(tmp_mat_4x4.arr, 1, 0);
	get_4x4(transp->arr, 0, 2) = get_4x4(tmp_mat_4x4.arr, 2, 0);
	get_4x4(transp->arr, 0, 3) = get_4x4(tmp_mat_4x4.arr, 3, 0);
	get_4x4(transp->arr, 1, 0) = get_4x4(tmp_mat_4x4.arr, 0, 1);
	get_4x4(transp->arr, 1, 1) = get_4x4(tmp_mat_4x4.arr, 1, 1);
	get_4x4(transp->arr, 1, 2) = get_4x4(tmp_mat_4x4.arr, 2, 1);
	get_4x4(transp->arr, 1, 3) = get_4x4(tmp_mat_4x4.arr, 3, 1);
	get_4x4(transp->arr, 2, 0) = get_4x4(tmp_mat_4x4.arr, 0, 2);
	get_4x4(transp->arr, 2, 1) = get_4x4(tmp_mat_4x4.arr, 1, 2);
	get_4x4(transp->arr, 2, 2) = get_4x4(tmp_mat_4x4.arr, 2, 2);
	get_4x4(transp->arr, 2, 3) = get_4x4(tmp_mat_4x4.arr, 3, 2);
	get_4x4(transp->arr, 3, 0) = get_4x4(tmp_mat_4x4.arr, 0, 3);
	get_4x4(transp->arr, 3, 1) = get_4x4(tmp_mat_4x4.arr, 1, 3);
	get_4x4(transp->arr, 3, 2) = get_4x4(tmp_mat_4x4.arr, 2, 3);
	get_4x4(transp->arr, 3, 3) = get_4x4(tmp_mat_4x4.arr, 3, 3);
	return transp;
}

static inline matrix_4x4* copy_to_mat4x4(const matrix_4x4* const base, matrix_4x4* const copy)
{
	get_4x4(copy->arr, 0, 0) = get_4x4(base->arr, 0, 0);
	get_4x4(copy->arr, 0, 1) = get_4x4(base->arr, 0, 1);
	get_4x4(copy->arr, 0, 2) = get_4x4(base->arr, 0, 2);
	get_4x4(copy->arr, 0, 3) = get_4x4(base->arr, 0, 3);
	get_4x4(copy->arr, 1, 0) = get_4x4(base->arr, 1, 0);
	get_4x4(copy->arr, 1, 1) = get_4x4(base->arr, 1, 1);
	get_4x4(copy->arr, 1, 2) = get_4x4(base->arr, 1, 2);
	get_4x4(copy->arr, 1, 3) = get_4x4(base->arr, 1, 3);
	get_4x4(copy->arr, 2, 0) = get_4x4(base->arr, 2, 0);
	get_4x4(copy->arr, 2, 1) = get_4x4(base->arr, 2, 1);
	get_4x4(copy->arr, 2, 2) = get_4x4(base->arr, 2, 2);
	get_4x4(copy->arr, 2, 3) = get_4x4(base->arr, 2, 3);
	get_4x4(copy->arr, 3, 0) = get_4x4(base->arr, 3, 0);
	get_4x4(copy->arr, 3, 1) = get_4x4(base->arr, 3, 1);
	get_4x4(copy->arr, 3, 2) = get_4x4(base->arr, 3, 2);
	get_4x4(copy->arr, 3, 3) = get_4x4(base->arr, 3, 3);
	return copy;
}

// #include <stdio.h>
// static inline void dump_mat4x4(const matrix_4x4* const matrix)
// {
// 	printf("%lf, %lf, %lf, %lf\n", get_4x4(matrix->arr, 0, 0), get_4x4(matrix->arr, 1, 0), get_4x4(matrix->arr, 2, 0), get_4x4(matrix->arr, 3, 0));
// 	printf("%lf, %lf, %lf, %lf\n", get_4x4(matrix->arr, 0, 1), get_4x4(matrix->arr, 1, 1), get_4x4(matrix->arr, 2, 1), get_4x4(matrix->arr, 3, 1));
// 	printf("%lf, %lf, %lf, %lf\n", get_4x4(matrix->arr, 0, 2), get_4x4(matrix->arr, 1, 2), get_4x4(matrix->arr, 2, 2), get_4x4(matrix->arr, 3, 2));
// 	printf("%lf, %lf, %lf, %lf\n", get_4x4(matrix->arr, 0, 3), get_4x4(matrix->arr, 1, 3), get_4x4(matrix->arr, 2, 3), get_4x4(matrix->arr, 3, 3));
// }

#endif
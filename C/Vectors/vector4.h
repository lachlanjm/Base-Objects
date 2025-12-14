#ifndef VECTOR_4_H
#define VECTOR_4_H

#include <stdlib.h>
#include "vector_standards.h"

typedef struct vector4 
{
	VECTOR_FLT arr[4];
} vector4;

static inline vector4* new_vec4(const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z, const VECTOR_FLT w)
{
	vector4* vector = (vector4*) calloc(1, sizeof(vector4));
	set_vec4(vector, x, y, z, w);
	return vector;
}
static inline void set_vec4(vector4* vector, const VECTOR_FLT x, const VECTOR_FLT y, const VECTOR_FLT z, const VECTOR_FLT w)
{
	get_vec4(vector->arr, 0) = x;
	get_vec4(vector->arr, 1) = y;
	get_vec4(vector->arr, 2) = z;
	get_vec4(vector->arr, 3) = w;
}

static inline void translate_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_vec4(vector->arr, 0) += delta_x * get_vec4(vector->arr, 3);
	get_vec4(vector->arr, 1) += delta_y * get_vec4(vector->arr, 3);
	get_vec4(vector->arr, 2) += delta_z * get_vec4(vector->arr, 3);
}
static inline void scale_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	get_vec4(vector->arr, 0) *= delta_x;
	get_vec4(vector->arr, 1) *= delta_y;
	get_vec4(vector->arr, 2) *= delta_z;
}

static inline void rotate_vec4_x_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = get_vec4(vector->arr, 1)*cos(delta) - get_vec4(vector->arr, 2)*sin(delta);
	get_vec4(vector->arr, 2) = get_vec4(vector->arr, 1)*sin(delta) + get_vec4(vector->arr, 2)*cos(delta);
	get_vec4(vector->arr, 1) = tmp;
}
static inline void rotate_vec4_y_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = get_vec4(vector->arr, 2)*cos(delta) - get_vec4(vector->arr, 0)*sin(delta);
	get_vec4(vector->arr, 0) = get_vec4(vector->arr, 2)*sin(delta) + get_vec4(vector->arr, 0)*cos(delta);
	get_vec4(vector->arr, 2) = tmp;
}
static inline void rotate_vec4_z_axis_rad(vector4* vector, const VECTOR_FLT delta)
{
	VECTOR_FLT tmp = get_vec4(vector->arr, 0)*cos(delta) - get_vec4(vector->arr, 1)*sin(delta);
	get_vec4(vector->arr, 1) = get_vec4(vector->arr, 0)*sin(delta) + get_vec4(vector->arr, 1)*cos(delta);
	get_vec4(vector->arr, 0) = tmp;
}
/*
	WILL APPLY ROTATION IN ORDER M = B.X.Y.Z
*/
static inline void rotate_vec4(vector4* vector, const VECTOR_FLT delta_x, const VECTOR_FLT delta_y, const VECTOR_FLT delta_z)
{
	VECTOR_FLT A = cos(delta_x);
	VECTOR_FLT B = sin(delta_x);
	VECTOR_FLT C = cos(delta_y);
	VECTOR_FLT D = sin(delta_y);
	VECTOR_FLT E = cos(delta_z);
	VECTOR_FLT F = sin(delta_z);

	VECTOR_FLT AD = A*D;
	VECTOR_FLT BD = B*D;

	VECTOR_FLT x = get_vec4(vector->arr, 0);
	VECTOR_FLT y = get_vec4(vector->arr, 1);
	VECTOR_FLT z = get_vec4(vector->arr, 2);

	get_vec4(vector->arr, 0) = x*C*E - y*C*F + z*D;
	get_vec4(vector->arr, 1) = x*(BD*E + A*F) + y*(-BD*F + A*E) - z*B*C;
	get_vec4(vector->arr, 2) = x*(-AD*E + B*F) + y*(AD*F + B*E) + z*A*C;
}

static inline void rotate_vec4_x_axis_deg(vector4* vector, const VECTOR_FLT delta)
{
	rotate_vec4_x_axis_rad(vector, deg_to_rad(delta));
}
static inline void rotate_vec4_y_axis_deg(vector4* vector, const VECTOR_FLT delta)
{
	rotate_vec4_y_axis_rad(vector, deg_to_rad(delta));
}
static inline void rotate_vec4_z_axis_deg(vector4* vector, const VECTOR_FLT delta)
{
	rotate_vec4_z_axis_rad(vector, deg_to_rad(delta));
}

static inline void transform_vec4_rad(vector4* vector, 
	const VECTOR_FLT scale_delta_x, const VECTOR_FLT scale_delta_y, const VECTOR_FLT scale_delta_z,
	const VECTOR_FLT rotate_delta_x, const VECTOR_FLT rotate_delta_y, const VECTOR_FLT rotate_delta_z,
	const VECTOR_FLT translate_delta_x, const VECTOR_FLT translate_delta_y, const VECTOR_FLT translate_delta_z
) {
	scale_vec4(vector, scale_delta_x, scale_delta_y, scale_delta_z);
	rotate_vec4(vector, rotate_delta_x, rotate_delta_y, rotate_delta_z);
	translate_vec4(vector, translate_delta_x, translate_delta_y, translate_delta_z);
}

static inline void transform_vec4_deg(vector4* vector, 
	const VECTOR_FLT scale_delta_x, const VECTOR_FLT scale_delta_y, const VECTOR_FLT scale_delta_z,
	const VECTOR_FLT rotate_delta_x, const VECTOR_FLT rotate_delta_y, const VECTOR_FLT rotate_delta_z,
	const VECTOR_FLT translate_delta_x, const VECTOR_FLT translate_delta_y, const VECTOR_FLT translate_delta_z
) {
	transform_vec4_rad(vector, 
		scale_delta_x, scale_delta_y, scale_delta_z,
		deg_to_rad(rotate_delta_x), deg_to_rad(rotate_delta_y), deg_to_rad(rotate_delta_z),
		translate_delta_x, translate_delta_y, translate_delta_z
	);
}


#endif
#ifndef VECTOR_STANDARDS_H
#define VECTOR_STANDARDS_H

#include <float.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef float VECTOR_FLT;

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define get_vec2(VEC2_ARR, ROW) (VEC2_ARR)[ROW]
#define get_vec3(VEC3_ARR, ROW) (VEC3_ARR)[ROW]
#define get_vec4(VEC4_ARR, ROW) (VEC4_ARR)[ROW]
#define get_2x2(MAT_2X2_ARR, COL, ROW) (MAT_2X2_ARR)[2*(COL) + ROW]
#define get_3x3(MAT_3X3_ARR, COL, ROW) (MAT_3X3_ARR)[3*(COL) + ROW]
#define get_4x4(MAT_4X4_ARR, COL, ROW) (MAT_4X4_ARR)[4*(COL) + ROW]
#define deg_to_rad(DEG) ((DEG) * (M_PI / 180.0f))

#define VECTOR_FLT_MIN FLT_MIN;
#define VECTOR_FLT_MAX FLT_MAX;

#endif
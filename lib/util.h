#ifndef RRAY_UTIL_H
#define RRAY_UTIL_H

#include "math.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define PI 3.14159265358979323846
#define NS_PER_SECOND INT64_C(1000000000)

typedef struct {
	float x, y, z;
} Vec3;

typedef struct {
	float arr[9];
} Mat3;

Vec3 solve_cramer(Mat3 A, Vec3 y);
float solve_cramerf(Mat3 A, Vec3 y, int ind);

static inline Vec3 vec3_zero() { return (Vec3){0, 0, 0}; }

static inline bool vec3_is_zero(Vec3 v) {
	return (v.x == 0 && v.y == 0 && v.z == 0);
}

static inline Vec3 vec3_of_arr(float arr[3]) {
	return (Vec3){
	    .x = arr[0],
	    .y = arr[1],
	    .z = arr[2],
	};
}

static inline Vec3 vec3_add(Vec3 A, Vec3 B) {
	return (Vec3){A.x + B.x, A.y + B.y, A.z + B.z};
}

static inline float vec3_dot(Vec3 A, Vec3 B) {
	return A.x * B.x + A.y * B.y + A.z * B.z;
}

static inline Vec3 vec3_cross(Vec3 A, Vec3 B) {
	return (Vec3){A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z,
	              A.x * B.y - A.y * B.x};
}

static inline Vec3 vec3_mulf(Vec3 A, float f) {
	return (Vec3){f * A.x, f * A.y, f * A.z};
}

static inline Vec3 vec3_sub(Vec3 A, Vec3 B) {
	return vec3_add(A, vec3_mulf(B, -1.0f));
}

static inline Vec3 vec3_reflect(Vec3 A, Vec3 B) {
	return vec3_sub(vec3_mulf(B, 2.0f * vec3_dot(B, A)), A);
}

static inline float vec3_norm(Vec3 a) { return sqrtf(vec3_dot(a, a)); }

static inline Vec3 vec3_unit(Vec3 a) { return vec3_mulf(a, 1 / vec3_norm(a)); }

static inline Mat3 mat3_zero() {
	return (Mat3){.arr = {0, 0, 0, 0, 0, 0, 0, 0, 0}};
}

static inline Mat3 mat3_one() {
	return (Mat3){.arr = {1, 0, 0, 0, 1, 0, 0, 0, 1}};
}

static inline Mat3 mat3_mul(Mat3 a, Mat3 b) {
	Mat3 acc = mat3_zero();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				acc.arr[i * 3 + j] += a.arr[i * 3 + k] * b.arr[k * 3 + j];
	return acc;
}

static inline Vec3 mat3_mulv(Mat3 A, Vec3 v) {
	float v_arr[3] = {v.x, v.y, v.z};
	float acc[3] = {0};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			acc[i] += A.arr[3 * i + j] * v_arr[j];

	return vec3_of_arr(acc);
}

static inline Mat3 mat3_rotx(float x) {
	return (Mat3){.arr = {1, 0, 0, 0, cosf(x), -sinf(x), 0, sinf(x), cosf(x)}};
}

static inline Mat3 mat3_roty(float x) {
	return (Mat3){.arr = {cosf(x), 0, sinf(x), 0, 1, 0, -sinf(x), 0, cosf(x)}};
}

static inline Mat3 mat3_rotz(float x) {
	return (Mat3){.arr = {cosf(x), -sinf(x), 0, sinf(x), cosf(x), 0, 0, 0, 1}};
}

static inline Mat3 mat3_rot(Vec3 r) {
	return mat3_mul(mat3_rotx(r.x), mat3_mul(mat3_roty(r.y), mat3_rotz(r.z)));
}

static inline int64_t ns_between(struct timespec a, struct timespec b) {
	return (b.tv_sec - a.tv_sec) * NS_PER_SECOND + (b.tv_nsec - a.tv_nsec);
}

static inline int mini(int a, int b) { return a < b ? a : b; }

static inline int maxi(int a, int b) { return a > b ? a : b; }

static inline int clampi(int v, int lo, int hi) {
	return mini(maxi(v, lo), hi);
}

static inline float minf(float a, float b) { return a < b ? a : b; }

static inline float maxf(float a, float b) { return a > b ? a : b; }

static inline float clampf(float v, float lo, float hi) {
	return minf(maxf(v, lo), hi);
}

static inline float equalf(float a, float b, float eps) {
	return fabsf(a - b) <= eps;
}

static inline Vec3 vec3_clamp(Vec3 v, float lo, float hi) {
	return (Vec3){.x = clampf(v.x, lo, hi),
	              .y = clampf(v.y, lo, hi),
	              .z = clampf(v.z, lo, hi)};
}

#endif
#ifndef RREN_UTIL_H
#define RREN_UTIL_H

#include "math.h"
#include "stdlib.h"

typedef struct {
	float x, y, z;
} Vec3;

static inline Vec3 vec3_add(Vec3 A, Vec3 B) {
	return (Vec3){A.x + B.x, A.y + B.y, A.z + B.z};
}

static inline float vec3_dot(Vec3 A, Vec3 B) {
	return A.x * B.x + A.y * B.y + A.z * B.z;
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

static inline float vec3_norm(Vec3 a) { return sqrt(vec3_dot(a, a)); }

static inline Vec3 vec3_unit(Vec3 a) { return vec3_mulf(a, 1 / vec3_norm(a)); }

static inline int imin(int a, int b) { return a < b ? a : b; }

static inline int imax(int a, int b) { return a > b ? a : b; }

static inline int iclamp(int v, int lo, int hi) {
	return imin(imax(v, lo), hi);
}

static inline float minf(float a, float b) { return a < b ? a : b; }

static inline float maxf(float a, float b) { return a > b ? a : b; }

static inline float clampf(float v, float lo, float hi) {
	return minf(maxf(v, lo), hi);
}

#endif
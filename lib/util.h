#ifndef RREN_UTIL_H
#define RREN_UTIL_H

#include "stdlib.h"

typedef struct {
	float x, y, z;
} Vec3;

static inline Vec3 vec3_add(Vec3 a, Vec3 b) {
	return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline float vec3_dot(Vec3 a, Vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Vec3 vec3_mulk(Vec3 a, float k) {
	return (Vec3){k * a.x, k * a.y, k * a.z};
}

static inline Vec3 vec3_sub(Vec3 a, Vec3 b) {
	return vec3_add(a, vec3_mulk(b, -1.0f));
}

static inline int imin(int a, int b) { return a < b ? a : b; }

static inline int imax(int a, int b) { return a > b ? a : b; }

static inline int iclamp(int v, int lo, int hi) {
	return imin(imax(v, lo), hi);
}

#endif
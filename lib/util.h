#ifndef RREN_UTIL_H
#define RREN_UTIL_H

#include "math.h"
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

static inline Vec3 vec3_muli(Vec3 a, float k) {
	return (Vec3){k * a.x, k * a.y, k * a.z};
}

static inline Vec3 vec3_sub(Vec3 a, Vec3 b) {
	return vec3_add(a, vec3_muli(b, -1.0f));
}

static inline float vec3_norm(Vec3 a) { return sqrt(vec3_dot(a, a)); }

static inline Vec3 vec3_unit(Vec3 a) { return vec3_muli(a, 1 / vec3_norm(a)); }

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
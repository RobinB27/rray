#ifndef RREN_UTIL_H
#define RREN_UTIL_H

#include "stdlib.h"

#define VLA_MAX_CAP 4096
#define VLA_DEFAULT_CAP 16

typedef struct {
	int *data;
	size_t len;
	size_t cap;
} Vla_Int;

void vla_free(Vla_Int *arr);
Vla_Int *vla_make();
int vla_push(Vla_Int *arr, int value);

static inline int imin(int a, int b) { return a < b ? a : b; }
static inline int imax(int a, int b) { return a > b ? a : b; }
static inline int iclamp(int v, int lo, int hi) {
	return imin(imax(v, lo), hi);
}

#endif
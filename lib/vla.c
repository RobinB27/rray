#include "vla.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void *vla_grow_(void *data, size_t *cap, size_t elem_size) {
	size_t new_cap = *cap ? *cap * 2 : 8;
	if (new_cap > VLA_MAX_CAP || new_cap > SIZE_MAX / elem_size) {
		fprintf(stderr, "Can't extend variable length array: limit reached\n");
		return data;
	}
	void *new_data = realloc(data, new_cap * elem_size);
	if (new_data == NULL) {
		fprintf(stderr, "Can't extend variable length array: out of memory\n");
		return data;
	}
	*cap = new_cap;
	return new_data;
}

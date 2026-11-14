#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Vla_Int *vla_make() {
	Vla_Int *arr;
	if (!(arr = (Vla_Int *)malloc(sizeof(Vla_Int)))) {
		fprintf(stderr, "Failed to allocate variable length array\n");
		return NULL;
	}

	arr->len = 0;
	arr->cap = VLA_DEFAULT_CAP;

	if (!(arr->data = (int *)malloc(arr->cap * sizeof(int)))) {
		fprintf(stderr, "Failed to allocate variable length array\n");
		free(arr);
		return NULL;
	}
	return arr;
}

int vla_push(Vla_Int *arr, int value) {
	if (arr->len == arr->cap) {
		if (arr->cap * 2 > VLA_MAX_CAP) {
			fprintf(stderr,
			        "Can't extend variable length array: limit reached\n");
			return EXIT_FAILURE;
		}
		int *new_data = realloc(arr->data, arr->cap * 2 * sizeof(int));
		if (new_data == NULL) {
			fprintf(stderr, "Can't extend variable length array: error\n");
			return EXIT_FAILURE;
		}
		arr->data = new_data;
		arr->cap *= 2;
	}
	arr->data[arr->len++] = value;
	return EXIT_SUCCESS;
}

void vla_free(Vla_Int *arr) {
	free(arr->data);
	free(arr);
}
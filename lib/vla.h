#ifndef RREN_VLA_H
#define RREN_VLA_H

#include <stddef.h>

// Generiv variable length array via cpp macros

#define VLA_MAX_CAP ((size_t)1 << 24)

#define VLA(T)                                                                 \
	struct {                                                                   \
		T *data;                                                               \
		size_t len;                                                            \
		size_t cap;                                                            \
	}

void *vla_grow_(void *data, size_t *cap, size_t elem_size);

#define vla_push(arr, value)                                                   \
	(((arr)->len < (arr)->cap ||                                               \
	  ((arr)->data = vla_grow_((arr)->data, &(arr)->cap, sizeof *(arr)->data), \
	   (arr)->len < (arr)->cap))                                               \
	     ? ((arr)->data[(arr)->len++] = (value), 0)                            \
	     : -1)

#define vla_free(arr)                                                          \
	(free((arr)->data), (arr)->data = NULL, (arr)->len = (arr)->cap = 0)

#endif

#include "scene.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Scene *scene_make() {
	Scene *scene;
	if (!(scene = (Scene *)malloc(sizeof(Scene)))) {
		fprintf(stderr, "Failed to allocate Scene\n");
		return NULL;
	}
	if (!(scene->elements = vla_shape_make())) {
		fprintf(stderr, "Failed to allocate Scene\n");
		free(scene);
		return NULL;
	}
	return scene;
}

void scene_free(Scene *scene) {
	vla_shape_free(scene->elements);
	free(scene);
}

Vla_Shape *vla_shape_make() {
	Vla_Shape *arr;
	if (!(arr = (Vla_Shape *)malloc(sizeof(Vla_Shape)))) {
		fprintf(stderr, "Failed to allocate variable length array\n");
		return NULL;
	}

	arr->len = 0;
	arr->cap = VLA_DEFAULT_CAP;

	if (!(arr->data = (Shape *)malloc(arr->cap * sizeof(Vla_Shape)))) {
		fprintf(stderr, "Failed to allocate variable length array\n");
		free(arr);
		return NULL;
	}
	return arr;
}

int vla_shape_push(Vla_Shape *arr, Shape value) {
	if (arr->len == arr->cap) {
		if (arr->cap * 2 > VLA_MAX_CAP) {
			fprintf(stderr,
			        "Can't extend variable length array: limit reached\n");
			return EXIT_FAILURE;
		}
		Shape *new_data =
		    (Shape *)realloc(arr->data, arr->cap * 2 * sizeof(Shape));
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

void vla_shape_free(Vla_Shape *arr) {
	free(arr->data);
	free(arr);
}

Scene *scene_example() {
    Shape sh_a = {
        SH_SPHERE,
        col_rgb(255u, 0u, 0u),
        {
            0.0f,
            -1.0f,
            3.0f,
            1.0f
        }
    };

    Shape sh_b = {
        SH_SPHERE,
        col_rgb(0u, 0u, 255u),
        {
            2.0f,
            0.0f,
            4.0f,
            1.0f
        }
    };

    Shape sh_c = {
        SH_SPHERE,
        col_rgb(0u, 255u, 0u),
        {
            -2.0f,
            0.0f,
            4.0f,
            1.0f
        }
    };

    Scene *scene = scene_make();
    vla_shape_push(scene->elements, sh_a);
    vla_shape_push(scene->elements, sh_b);
    vla_shape_push(scene->elements, sh_c);
    
    return scene;
}
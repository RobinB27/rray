#include "scene.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Scene *scene_make() {
	Scene *scene;
	if (!(scene = (Scene *)malloc(sizeof(Scene)))) {
		fprintf(stderr, "Failed to allocate Scene\n");
		return NULL;
	}
	// calloc is critical so len, cap init. to 0
	if (!(scene->elements = (VlaShape *)calloc(1, sizeof(VlaShape)))) {
		fprintf(stderr, "Failed to allocate Scene\n");
		free(scene);
		return NULL;
	}
	if (!(scene->lights = (VlaLight *)calloc(1, sizeof(VlaLight)))) {
		fprintf(stderr, "Failed to allocate Scene\n");
		free(scene->elements);
		free(scene);
		return NULL;
	}
	return scene;
}

void scene_free(Scene *scene) {
	vla_free(scene->elements);
	vla_free(scene->lights);
	free(scene->elements);
	free(scene->lights);
	free(scene);
}

Scene *scene_example() {
	Scene *scene;
	if (!(scene = scene_make())) {
		fprintf(stderr, "Failed to create example scene\n");
		return NULL;
	}

	scene->i_ambient = 0.2;

	vla_push(scene->elements, make_sphere((Vec3){0, -1, 3}, 1.0f,
	                                      col_rgb(255u, 0u, 0u), 1500, 0.2));
	vla_push(scene->elements, make_sphere((Vec3){2, 0, 4}, 1.0f,
	                                      col_rgb(0u, 0u, 255u), 500, 0.3));
	vla_push(scene->elements, make_sphere((Vec3){-2, 0, 4}, 1.0f,
	                                      col_rgb(0u, 255u, 0u), 10, 0.4));

	vla_push(scene->elements, make_tri((Vec3){-200, -1, 0.5},
	                                   (Vec3){0, -1, 400}, (Vec3){200, -1, 0.5},
	                                   col_rgb(255u, 255u, 0u), 1000, 0.2));
	vla_push(scene->elements,
	         make_tri((Vec3){-0.3, 1.5, 6}, (Vec3){0, 2.1, 6},
	                  (Vec3){0.3, 1.5, 6}, col_rgb(128u, 128u, 128u), 10, 0.2));

	vla_push(scene->lights, make_plight((Vec3){2, 1, 0}, 0.6f));
	vla_push(scene->lights, make_dlight((Vec3){1, 4, 4}, 0.2f));

	return scene;
}
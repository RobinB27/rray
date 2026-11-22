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

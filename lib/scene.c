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
	if (!(scene->elements = (Vla_Shape *)calloc(1, sizeof(Vla_Shape)))) {
		fprintf(stderr, "Failed to allocate Scene\n");
		free(scene);
		return NULL;
	}
	if (!(scene->lights = (Vla_Light *)calloc(1, sizeof(Vla_Light)))) {
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
	Shape sh_a = {.type=SH_SPHERE, .color=col_rgb(255u, 0u, 0u), .Sphere={.pos={0.0f, -1.0f, 3.0f}, .r=1.0f}};

	Shape sh_b = {.type=SH_SPHERE, .color=col_rgb(0u, 0u, 255u), .Sphere={.pos={2.0f, 0.0f, 4.0f}, .r=1.0f}};

	Shape sh_c = {.type=SH_SPHERE, .color=col_rgb(0u, 255u, 0u), .Sphere={.pos={-2.0f, 0.0f, 4.0f}, .r=1.0f}};

	Scene *scene;
	if (!(scene = scene_make())) {
		fprintf(stderr, "Failed to create example scene\n");
		return NULL;
	}
	vla_push(scene->elements, sh_a);
	vla_push(scene->elements, sh_b);
	vla_push(scene->elements, sh_c);

	return scene;
}
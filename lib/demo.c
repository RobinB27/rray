#include "demo.h"
#include "scene.h"
#include "util.h"
#include <stdio.h>

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

/**
    Update the scene to move the light.
*/
void update_example(Scene *scene, float dt) {
    static bool flip = false;
    float pos = (scene->lights->data)[0].Point.pos.x;

    if (pos >= 1.5f) flip = true;
    else if (pos <= -1.5f) flip = false;

    float amt = minf(0.1, dt / NS_PER_SECOND);

    if (flip)
        (scene->lights->data)[0].Point.pos.x -= amt;
    else
        (scene->lights->data)[0].Point.pos.x += amt;
}
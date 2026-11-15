#include "raytracer.h"
#include "col.h"
#include "framebuf.h"
#include "scene.h"
#include "util.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static inline Vec3 canvas_to_viewport(int x, int y) {
	return (Vec3){(float)x * VIEWPORT_WIDTH / FB_WIDTH,
	              (float)y * VIEWPORT_HEIGHT / FB_HEIGHT, VIEWPORT_DIST};
}

Ray_Result intersect_ray_sphere(Vec3 O, Vec3 D, Shape *shape) {
	if (shape->type != SH_SPHERE) {
		fprintf(stderr, "intersect_ray_sphere got non-sphere shape\n");
		exit(EXIT_FAILURE);
	}

	// Solve quadr. eq. for ray & sphere intersection

	float r = shape->Sphere.r;
	Vec3 CO = vec3_sub(O, shape->Sphere.pos);

	float a = vec3_dot(D, D), b = 2 * vec3_dot(CO, D),
	      c = vec3_dot(CO, CO) - r * r;

	float disc = b * b - 4 * a * c;
	if (disc < 0)
		return (Ray_Result){TRACE_LIMIT, TRACE_LIMIT};

	return (Ray_Result){(-b + sqrt(disc)) / (2 * a),
	                    (-b - sqrt(disc)) / (2 * a)};
}

col_t trace_ray(Scene *scene, Vec3 O, Vec3 D, float t_min, float t_max) {
	float closest_t = TRACE_LIMIT;
	Shape *closest_sh = NULL;

	for (unsigned i = 0; i < scene->elements->len; i++) {
		Shape *shape = &scene->elements->data[i];
		if (shape->type != SH_SPHERE)
			continue;

		Ray_Result res = intersect_ray_sphere(O, D, shape);

		if (t_min < res.t1 && res.t1 < t_max && res.t1 < closest_t) {
			closest_t = res.t1;
			closest_sh = shape;
		}
		if (t_min < res.t2 && res.t2 < t_max && res.t2 < closest_t) {
			closest_t = res.t2;
			closest_sh = shape;
		}
	}

	if (closest_sh == NULL)
		return C_BLACK;

	return closest_sh->color;
}

int render(framebuf *fb, Scene *scene) {
	Vec3 O = {0, 0, 0};

	for (int x = -FB_WIDTH / 2; x <= FB_WIDTH / 2; x++) {
		for (int y = -FB_HEIGHT / 2; y <= FB_HEIGHT / 2; y++) {
			Vec3 D = canvas_to_viewport(x, y);
			col_t color = trace_ray(scene, O, D, 1, TRACE_LIMIT);
			fb_putc(fb, color, x, y);
		}
	}

	return EXIT_SUCCESS;
}

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

Intersect_Result closest_intersection(Scene *scene, Vec3 O, Vec3 D, float t_min, float t_max) {
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
    return (Intersect_Result){.sh=closest_sh, .t=closest_t};
}

/**
    Find ray intersection vectors for a sphere.

    @param O        Camera location
    @param D        Ray direction
    @param shape    Sphere
    @return         Both possible intersections
*/
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

/**
    Compute intensity using diffuse & specular reflections.

    @param scene    Scene that provides Light objects
    @param P        Surface position
    @param N        Surface normal
    @param S        Vector from P to camera
    @return         Intensity as a float in [0,1]
*/
float compute_lighting(Scene *scene, Vec3 P, Vec3 N, Vec3 V, int s) {
	float intensity = 0.0;

	intensity += scene->i_ambient;

	for (unsigned i = 0; i < scene->lights->len; i++) {
		Light *light = &scene->lights->data[i];

		Vec3 L = (Vec3){0, 0, 0};
		switch (light->type) {
		case L_POINT:
			L = vec3_sub(light->Point.pos, P);
			break;
		case L_DIRECTIONAL:
			L = light->Directional.dir;
			break;
		default:
			fprintf(stderr, "Light has invalid type\n");
			exit(EXIT_FAILURE);
		}

        // Shadow: check if something blocks the light
        Intersect_Result res = closest_intersection(scene, P, L, TRACE_EPSILON, TRACE_LIMIT);
        if (res.sh != NULL)
            continue;

		// Diffuse
		float dot_NL = vec3_dot(N, L);
		if (dot_NL > 0) {
			intensity += light->i * dot_NL / (vec3_norm(N) * vec3_norm(L));
		}

		// Specular
		if (s != -1) {
			Vec3 R = vec3_reflect(N, L);
			float dot_RV = vec3_dot(R, V);
			if (dot_RV > 0)
				intensity +=
				    light->i * pow(dot_RV / (vec3_norm(R) * vec3_norm(V)), s);
		}
	}

	return intensity;
}

/**
    Compute pixel color via raytracing.

    @param scene    Scene with lights & shapes
    @param O        Camera location
    @param D        Ray direction
    @param t_min    Minimum ray distance
    @param t_max    Maximum ray distance
*/
col_t trace_ray(Scene *scene, Vec3 O, Vec3 D, float t_min, float t_max, int rec_depth) {
	Intersect_Result res = closest_intersection(scene, O, D, t_min, t_max);
	if (res.sh == NULL)
		return C_BLACK;

	// Compute intersection P and normal N
	// Warning: Only supports Spheres
	Vec3 P = vec3_add(O, vec3_mulf(D, res.t));
	Vec3 N = vec3_sub(P, res.sh->Sphere.pos);
	N = vec3_unit(N);

	col_t local_col = col_mulf(res.sh->color,
	                compute_lighting(scene, P, N, vec3_mulf(D, -1.0f),
	                                 res.sh->specular));
    
    float r = res.sh->reflective;
    if (rec_depth <= 0 || r <= 0)
        return local_col;

    Vec3 R = vec3_reflect(vec3_mulf(D, -1.0f), N);
    col_t refl_col = trace_ray(scene, P, R, TRACE_EPSILON, TRACE_LIMIT, rec_depth-1);

    return col_add(col_mulf(local_col, 1-r), col_mulf(refl_col, r));
}

int render(framebuf *fb, Scene *scene) {
	Vec3 O = {0, 0, 0};

	for (int x = -FB_WIDTH / 2; x <= FB_WIDTH / 2; x++) {
		for (int y = -FB_HEIGHT / 2; y <= FB_HEIGHT / 2; y++) {
			Vec3 D = canvas_to_viewport(x, y);
			col_t color = trace_ray(scene, O, D, 1, TRACE_LIMIT, TRACE_REC_DEPTH);
			fb_putc(fb, color, x, y);
		}
	}

	return EXIT_SUCCESS;
}

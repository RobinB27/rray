#include "raytracer.h"
#include "col.h"
#include "framebuf.h"
#include "scene.h"
#include "util.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SPECULAR_MATTE -1

static inline Vec3 canvas_to_viewport(int x, int y) {
	return (Vec3){(float)x * VIEWPORT_WIDTH / FB_WIDTH,
	              (float)y * VIEWPORT_HEIGHT / FB_HEIGHT, VIEWPORT_DIST};
}

IntersectResult closest_intersection(Scene *scene, Vec3 O, Vec3 D, float t_min,
                                     float t_max) {
	float closest_t = TRACE_LIMIT;
	Shape *closest_sh = NULL;

	for (unsigned i = 0; i < scene->elements->len; i++) {
		Shape *shape = &scene->elements->data[i];

		RayResult res = (RayResult){.t1 = 0.0f, .t2 = 0.0f};
		switch (shape->type) {
		case SH_SPHERE:
			res = intersect_ray_sphere(O, D, shape);
			break;
		case SH_TRI:
			res = intersect_ray_tri(O, D, shape);
			break;
		}

		if (t_min < res.t1 && res.t1 < t_max && res.t1 < closest_t) {
			closest_t = res.t1;
			closest_sh = shape;
		}
		if (t_min < res.t2 && res.t2 < t_max && res.t2 < closest_t) {
			closest_t = res.t2;
			closest_sh = shape;
		}
	}
	return (IntersectResult){.sh = closest_sh, .t = closest_t};
}

/**
    Find ray intersection vectors for a sphere.

    @param O        Ray origin
    @param D        Ray direction
    @param shape    Sphere
    @return         Both possible intersections, or 0 for no intersection
*/
RayResult intersect_ray_sphere(Vec3 O, Vec3 D, Shape *shape) {
	// Solve quadr. eq. for ray & sphere intersection
	// CGfS Ch. 2

	float r = shape->Sphere.r;
	Vec3 CO = vec3_sub(O, shape->Sphere.pos);

	float a = vec3_dot(D, D), b = 2 * vec3_dot(CO, D),
	      c = vec3_dot(CO, CO) - r * r;

	float disc = b * b - 4 * a * c;
	if (disc < 0)
		return (RayResult){0, 0};

	return (RayResult){.t1 = (-b + sqrtf(disc)) / (2 * a),
	                   .t2 = (-b - sqrtf(disc)) / (2 * a)};
}

/**
    Find ray intersection vectors for a triangle.

    @param O        Camera location
    @param D        Ray direction
    @param shape    Triangle
    @return         Single intersection, or 0 if no intersection
*/
RayResult intersect_ray_tri(Vec3 O, Vec3 D, Shape *shape) {
	// Solve linear system for ray & tri intersection
	// FCG Ch. 4.4

	Vec3 A = shape->Tri.A, B = shape->Tri.B, C = shape->Tri.C;
	Mat3 M = (Mat3){.arr = {A.x - B.x, A.x - C.x, D.x, A.y - B.y, A.y - C.y,
	                        D.y, A.z - B.z, A.z - C.z, D.z}};
	Vec3 Y = (Vec3){A.x - O.x, A.y - O.y, A.z - O.z};

	// results x,y are barycentric coords on triangle. z is intersection's t
	Vec3 res = solve_cramer(M, Y);

	// Point on plane isn't on triangle
	if (res.x < 0 || res.y < 0 || res.x + res.y > 1)
		return (RayResult){.t1 = 0.0f, .t2 = 0.0f};

	return (RayResult){.t1 = res.z, .t2 = res.z};
}

/**
    Compute intensity using diffuse & specular reflections.

    @param scene    Scene that provides Light objects
    @param P        Surface position
    @param N        Surface normal
    @param V        Vector from P to camera
    @param s		Specular reflection strength
    @return         Intensity
*/
float compute_lighting(Scene *scene, Vec3 P, Vec3 N, Vec3 V, int s) {
	float intensity = 0.0;

	intensity += scene->i_ambient;

	for (unsigned i = 0; i < scene->lights->len; i++) {
		Light *light = &scene->lights->data[i];

		Vec3 L = (Vec3){0, 0, 0};
		IntersectResult res;
		switch (light->type) {
		case L_POINT:
			L = vec3_sub(light->Point.pos, P);
			res = closest_intersection(scene, P, L, TRACE_EPSILON, 1);
			break;
		case L_DIRECTIONAL:
			L = light->Directional.dir;
			res = closest_intersection(scene, P, L, TRACE_EPSILON, TRACE_LIMIT);
			break;
		default:
			fprintf(stderr, "Light has invalid type\n");
			exit(EXIT_FAILURE);
		}

		// Shadow: check if something blocks the light
		if (res.sh != NULL)
			continue;

		// Diffuse
		float dot_NL = vec3_dot(N, L);
		if (dot_NL > 0) {
			intensity += light->i * dot_NL / (vec3_norm(N) * vec3_norm(L));
		}

		// Specular
		if (s != SPECULAR_MATTE) {
			Vec3 R = vec3_reflect(N, L);
			float dot_RV = vec3_dot(R, V);
			if (dot_RV > 0)
				intensity +=
				    light->i * powf(dot_RV / (vec3_norm(R) * vec3_norm(V)), s);
		}
	}

	return intensity;
}

/**
    Compute pixel color via raytracing.

    @param scene    	Scene with lights & shapes
    @param O        	Camera location
    @param D        	Ray direction
    @param t_min    	Minimum ray distance
    @param t_max    	Maximum ray distance
    @param rec_depth	Maximum recursion for raytracing
*/
Color trace_ray(Scene *scene, Vec3 O, Vec3 D, float t_min, float t_max,
                int rec_depth) {
	IntersectResult res = closest_intersection(scene, O, D, t_min, t_max);
	if (res.sh == NULL)
		return C_BLACK;

	// Compute intersection P and normal N
	Vec3 P, N;
	P = vec3_add(O, vec3_mulf(D, res.t));
	switch (res.sh->type) {
	case (SH_SPHERE):
		N = vec3_sub(P, res.sh->Sphere.pos);
		break;
	case (SH_TRI):
		N = vec3_cross(vec3_sub(res.sh->Tri.B, res.sh->Tri.A),
		               vec3_sub(res.sh->Tri.C, res.sh->Tri.A));
		break;
	}
	N = vec3_unit(N);

	Color local_col = col_mulf(
	    res.sh->color,
	    compute_lighting(scene, P, N, vec3_mulf(D, -1.0f), res.sh->specular));

	float r = res.sh->reflective;
	if (rec_depth <= 0 || r <= 0)
		return local_col;

	Vec3 R = vec3_reflect(vec3_mulf(D, -1.0f), N);
	Color refl_col =
	    trace_ray(scene, P, R, TRACE_EPSILON, TRACE_LIMIT, rec_depth - 1);

	return col_add(col_mulf(local_col, 1 - r), col_mulf(refl_col, r));
}

int render(FrameBuffer *fb, Camera camera, Scene *scene) {
	Mat3 R = mat3_rot(camera.rot);
	Vec3 O = camera.pos;

	// Run multithreaded, if compiler supports it
	#ifdef _OPENMP
	#pragma omp parallel for
	#endif
	for (int x = -FB_WIDTH / 2; x < FB_WIDTH / 2; x++) {
		for (int y = -FB_HEIGHT / 2; y < FB_HEIGHT / 2; y++) {
			Vec3 D = canvas_to_viewport(x, y);
			D = mat3_mulv(R, D);
			Color color =
			    trace_ray(scene, O, D, 1, TRACE_LIMIT, TRACE_REC_DEPTH);
			fb_putc(fb, color, x, y);
		}
	}

	return EXIT_SUCCESS;
}

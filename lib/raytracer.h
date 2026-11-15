#ifndef RREN_RAYTRACER_H
#define RREN_RAYTRACER_H

#include "framebuf.h"
#include "scene.h"
#include "util.h"

#define VIEWPORT_WIDTH 2
#define VIEWPORT_HEIGHT 1
#define VIEWPORT_DIST 1 // 'd' in CGFS

#define TRACE_LIMIT 1000000.0f
#define TRACE_EPSILON 0.001
#define TRACE_REC_DEPTH 3

typedef struct {
	float t1;
	float t2;
} Ray_Result;

typedef struct {
    Shape *sh;
    float t;
} Intersect_Result;

int render(framebuf *fb, Scene *scene);
float compute_lighting(Scene *scene, Vec3 P, Vec3 N, Vec3 V, int s);
Intersect_Result closest_intersection(Scene *scene, Vec3 O, Vec3 D, float t_min, float t_max);
Ray_Result intersect_ray_sphere(Vec3 O, Vec3 D, Shape *shape);
col_t trace_ray(Scene *scene, Vec3 O, Vec3 D, float t_min, float t_max, int rec_depth);

#endif
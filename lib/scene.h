#ifndef RRAY_SCENE_H
#define RRAY_SCENE_H

#include "col.h"
#include "util.h"
#include "vla.h"
#include <stdlib.h>

typedef enum { SH_SPHERE, SH_TRI } ShapeType;

typedef struct {
	ShapeType type;
	Color color;
	int specular;
	float reflective;
	union {
		struct {
			Vec3 pos;
			float r;
		} Sphere;
		struct {
			Vec3 A;
			Vec3 B;
			Vec3 C;
		} Tri;
	};
} Shape;

typedef enum { L_POINT, L_DIRECTIONAL } LightType;

typedef struct {
	LightType type;
	float i;
	union {
		struct {
			Vec3 pos;
		} Point;
		struct {
			Vec3 dir;
		} Directional;
	};
} Light;

typedef VLA(Shape) VlaShape;
typedef VLA(Light) VlaLight;

typedef struct {
	float i_ambient;
	VlaShape *elements;
	VlaLight *lights;
} Scene;

static inline Shape make_sphere(Vec3 pos, float r, Color color, int specular,
                                float reflective) {
	return (Shape){.type = SH_SPHERE,
	               .color = color,
	               .specular = specular,
	               .reflective = reflective,
	               .Sphere = {.pos = pos, .r = r}};
}

static inline Shape make_tri(Vec3 A, Vec3 B, Vec3 C, Color color, int specular,
                             float reflective) {
	return (Shape){.type = SH_TRI,
	               .color = color,
	               .specular = specular,
	               .reflective = reflective,
	               .Tri = {A, B, C}};
}

static inline Light make_plight(Vec3 pos, float i) {
	return (Light){.type = L_POINT, .i = i, .Point = {.pos = pos}};
}

static inline Light make_dlight(Vec3 dir, float i) {
	return (Light){.type = L_DIRECTIONAL, .i = i, .Directional = {.dir = dir}};
}

Scene *scene_make();
void scene_free(Scene *scene);

#endif
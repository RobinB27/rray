#ifndef RREN_SCENE_H
#define RREN_SCENE_H

#include "col.h"
#include "util.h"
#include "vla.h"
#include <stdlib.h>

typedef enum { SH_SPHERE, SH_CUBE } ShapeType;

typedef struct {
	ShapeType type;
	col_t color;
	union {
		struct {
			Vec3 pos;
			float r;
		} Sphere;
		struct {
			Vec3 pos;
			float w;
			float h;
			float l;
		} Cube;
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

typedef VLA(Shape) Vla_Shape;
typedef VLA(Light) Vla_Light;

typedef struct {
	float i_ambient;
	Vla_Shape *elements;
	Vla_Light *lights;
} Scene;

static inline Shape make_sphere(Vec3 pos, float r, col_t color) {
	return (Shape){.type=SH_SPHERE, .color=color, .Sphere={.pos=pos, .r=r}};
}

static inline Light make_plight(Vec3 pos, float i) {
	return (Light){.type=L_POINT, .i=i, .Point={.pos=pos}};
}

static inline Light make_dlight(Vec3 dir, float i) {
	return (Light){.type=L_DIRECTIONAL, .i=i, .Directional={.dir=dir}};
}

Scene *scene_make();
void scene_free(Scene *scene);
Scene *scene_example();

#endif
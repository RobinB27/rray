#ifndef RREN_SCENE_H
#define RREN_SCENE_H

#include "col.h"
#include <stdlib.h>

typedef struct {
	float x, y, z;
} Vec3;

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

typedef struct {
	Shape *data;
	size_t len;
	size_t cap;
} Vla_Shape;

typedef struct {
	Vla_Shape *elements;
} Scene;

static inline Vec3 vec_add(Vec3 a, Vec3 b) {
    return (Vec3){
        a.x+b.x,
        a.y+b.y,
        a.z+b.z
    };
}

static inline float vec_dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline Vec3 vec_mulk(Vec3 a, float k) {
    return (Vec3){
        k*a.x,
        k*a.y,
        k*a.z
    };
}

static inline Vec3 vec_sub(Vec3 a, Vec3 b) {
    return vec_add(a, vec_mulk(b, -1.0f));
}

Scene *scene_make();
void scene_free(Scene *scene);
Scene *scene_example();

Vla_Shape *vla_shape_make();
int vla_shape_push(Vla_Shape *arr, Shape value);
void vla_shape_free(Vla_Shape *arr);

#endif
#ifndef RREN_RAYTRACER_H
#define RREN_RAYTRACER_H

#include "framebuf.h"
#include "scene.h"

typedef struct {
    float t1;
    float t2;
} Ray_Result;

int render(framebuf *fb, Scene *scene);

#endif
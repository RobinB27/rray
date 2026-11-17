#ifndef RREN_FRAMEBUF_H
#define RREN_FRAMEBUF_H

#include "col.h"
#include <stdint.h>
#include <stdlib.h>

#define FB_WIDTH 1280
#define FB_HEIGHT 720
#define W_SCALE 1

typedef struct {
	int width;
	int height;
	Color *buf;
} FrameBuffer;

FrameBuffer *fb_make(int width, int height);
void fb_free(FrameBuffer *fb);
void fb_clear(FrameBuffer *fb, Color color);
int fb_rect(FrameBuffer *fb, Color color, int x, int y, int w, int h);

static inline int fb_put(FrameBuffer *fb, Color color, int x, int y) {
	if (x < fb->width && y < fb->height && x >= 0 && y >= 0) {
		fb->buf[y * fb->width + x] = color;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

static inline int fb_putc(FrameBuffer *fb, Color color, int x, int y) {
	return fb_put(fb, color, FB_WIDTH / 2 + x, FB_HEIGHT / 2 - y);
}

#endif
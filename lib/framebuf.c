#include "framebuf.h"
#include "util.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

framebuf *fb_make(int width, int height) {
	if (width <= 0 || height <= 0) {
		fprintf(stderr, "Couldn't make framebuffer: width=%d, height=%d", width,
		        height);
		return NULL;
	}

	framebuf *fb;
	col_t *buf;

	if (!(fb = (framebuf *)calloc(1, sizeof(*fb)))) {
		fprintf(stderr, "Failed to allocate framebuffer\n");
		return NULL;
	}
	if (!(buf = (uint32_t *)calloc((size_t)width * height, sizeof(uint32_t)))) {
		fprintf(stderr, "Failed to allocate framebuffer\n");
		free(fb);
		return NULL;
	}

	fb->width = width;
	fb->height = height;
	fb->buf = buf;

	return fb;
}

void fb_free(framebuf *fb) {
	free(fb->buf);
	free(fb);
}

void fb_clear(framebuf *fb, col_t color) {
	for (int i = 0; i < fb->height; i++)
		for (int j = 0; j < fb->width; j++)
			fb->buf[i * fb->width + j] = color;
}

int fb_rect(framebuf *fb, col_t color, int x, int y, int w, int h) {
	if (x >= fb->width || y >= fb->height || x + w < 0 || y + h < 0)
		return EXIT_FAILURE;

	int limx = imin(x + w, fb->width);
	int limy = imin(y + h, fb->height);

	for (int i = imax(y, 0); i < limy; i++)
		for (int j = imax(x, 0); j < limx; j++)
			fb->buf[i * fb->width + j] = color;
	return EXIT_SUCCESS;
}
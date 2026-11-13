#include "framebuf.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

framebuf *fb_make(uint32_t width, uint32_t height) {
    framebuf *fb; 
    uint32_t *buf;
    if (!(fb = (framebuf*)calloc(1, sizeof(*fb)))) {
        fprintf(stderr, "Failed to allocate framebuffer\n");
        return NULL;
    }
    if (!(buf = (uint32_t*)calloc((size_t)width*height, sizeof(uint32_t)))) {
        fprintf(stderr, "Failed to allocate framebuffer\n");
        free(fb);
        return NULL;
    }
    fb->width=width;
    fb->height=height;
    fb->buf=buf;
    return fb;
}

void fb_free(framebuf *fb) {
    free(fb->buf);
    free(fb);
}

void fb_clear(framebuf *fb, col_t color) {
    for (unsigned i = 0; i < fb->width; i++)
        for (unsigned j = 0; j < fb->height; j++)
            fb->buf[i * fb->width + j] = color;
}

void fb_rect(framebuf *fb, col_t color, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    if (x >= fb->width || y >= fb->height)
        return;
    uint32_t limx = x+w < fb->width ? x+w : fb->width;
    uint32_t limy = y+h < fb->height ? y+h : fb->height;
    for (unsigned i = y; i < limy; i++)
        for (unsigned j = x; j < limx; j++)
            fb->buf[i * fb->width + j] = color;
}
#ifndef RREN_FRAMEBUF_H
#define RREN_FRAMEBUF_H

#include <stdint.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t *buf;
} framebuf;

typedef uint32_t col_t;

framebuf *fb_make(uint32_t width, uint32_t height);
void fb_free(framebuf *fb);
void fb_clear(framebuf *fb, col_t color);
void fb_rect(framebuf *fb, col_t color, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

static inline col_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return 0xFF000000u | (uint32_t)r << 16 | (uint32_t)g << 8 | b;
}

static inline void fb_put(framebuf *fb, col_t color, uint32_t x, uint32_t y) {
    if (x < fb->width && y < fb->height) 
        fb->buf[y*fb->width + x] = color;
}

#endif
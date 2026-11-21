#ifndef RRAY_COL_H
#define RRAY_COL_H

#include "util.h"
#include <stdint.h>

typedef uint32_t Color;

#define C_BLACK 0xFF000000u
#define C_WHITE 0xFFFFFFFFu

#define CH_R(col) ((uint8_t)(((col) >> 16) & 0xFFu))
#define CH_G(col) ((uint8_t)(((col) >> 8) & 0xFFu))
#define CH_B(col) ((uint8_t)((col) & 0xFFu))

static inline Color col_rgb(uint8_t r, uint8_t g, uint8_t b) {
	return 0xFF000000u | (uint32_t)r << 16 | (uint32_t)g << 8 | b;
}

static inline uint8_t ch_mul(uint8_t c, uint8_t k) {
	unsigned t = c * k + 128;
	return (uint8_t)((t + (t >> 8)) >> 8); // (c * k + 127) / 255
}

static inline uint8_t ch_add(uint8_t c, uint8_t k) {
	unsigned s = c + k;
	return (uint8_t)(s > 255 ? 255 : s);
}

static inline Color col_mul(Color color, uint8_t k) {
	return col_rgb(ch_mul(CH_R(color), k), ch_mul(CH_G(color), k),
	               ch_mul(CH_B(color), k));
}

static inline uint8_t ch_from_float(float v) {
	if (!(v > 0.0f))
		return 0;
	if (v >= 255.0f)
		return 255;
	return (uint8_t)(v + 0.5f); // Add 0.5f to round to nearest int
}

static inline Color col_mulf(Color color, float f) {
	return col_rgb(ch_from_float(CH_R(color) * f),
	               ch_from_float(CH_G(color) * f),
	               ch_from_float(CH_B(color) * f));
}

static inline Color col_addk(Color color, uint8_t k) {
	return col_rgb(ch_add(CH_R(color), k), ch_add(CH_G(color), k),
	               ch_add(CH_B(color), k));
}

static inline Color col_add(Color a, Color b) {
	return col_rgb(ch_add(CH_R(a), CH_R(b)), ch_add(CH_G(a), CH_G(b)),
	               ch_add(CH_B(a), CH_B(b)));
}

#endif
#include <types.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

uint32_t fb_init(uint32_t width, uint32_t height);
void fb_set_pixel(uint32_t x, uint32_t y, uint32_t color);

#define COLOR(r, g, b, a)   ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF)

#endif
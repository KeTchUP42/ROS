#ifndef TVGA_H
#define TVGA_H

#include <kernel/vga/vga.h>

#include <stdint.h>

/* Hardware text mode color constants. */
enum tvga_color
{
    TVGA_COLOR_BLACK = 0,
    TVGA_COLOR_BLUE = 1,
    TVGA_COLOR_GREEN = 2,
    TVGA_COLOR_CYAN = 3,
    TVGA_COLOR_RED = 4,
    TVGA_COLOR_MAGENTA = 5,
    TVGA_COLOR_BROWN = 6,
    TVGA_COLOR_LIGHT_GREY = 7,
    TVGA_COLOR_DARK_GREY = 8,
    TVGA_COLOR_LIGHT_BLUE = 9,
    TVGA_COLOR_LIGHT_GREEN = 10,
    TVGA_COLOR_LIGHT_CYAN = 11,
    TVGA_COLOR_LIGHT_RED = 12,
    TVGA_COLOR_LIGHT_MAGENTA = 13,
    TVGA_COLOR_LIGHT_BROWN = 14,
    TVGA_COLOR_WHITE = 15,
};

#define VGA_TEXT ((void*)(0xB8000))
#define VGA_TEXT_WIDTH (80)
#define VGA_TEXT_HEIGHT (25)

const vga_entry_type *system_text_vga(void);

static inline uint8_t tvga_entry_color(enum tvga_color fg, enum tvga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t tvga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

#endif // TVGA_H

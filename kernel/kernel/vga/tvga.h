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
#define VGA_TEXT_COLOR_TYPE uint8_t
#define VGA_TEXT_CELL_TYPE uint16_t

const vga_entry_type *system_text_vga(void);

static inline VGA_TEXT_COLOR_TYPE tvga_entry_color(enum tvga_color fg, enum tvga_color bg)
{
    return fg | (bg << ((sizeof(VGA_TEXT_COLOR_TYPE) * 8) / 2));
}

static inline VGA_TEXT_CELL_TYPE tvga_entry(unsigned char uc, VGA_TEXT_COLOR_TYPE color)
{
    return ((VGA_TEXT_CELL_TYPE) uc) | (((VGA_TEXT_CELL_TYPE) color) << ((sizeof(VGA_TEXT_CELL_TYPE) * 8) / 2));
}

#endif // TVGA_H

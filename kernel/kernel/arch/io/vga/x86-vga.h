#ifndef IO_VGA_X86_VGA_H
#define IO_VGA_X86_VGA_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include <kernel/arch/io/ports/x86-ports.h>
#include <stdint.h>

#define VGA_TEXT ((void*)(0xB8000))
#define VGA_TEXT_WIDTH (80)
#define VGA_TEXT_HEIGHT (25)
#define VGA_TEXT_COLOR_TYPE uint8_t
#define VGA_TEXT_CELL_TYPE uint16_t
#define VGA_TEXT_CURSOR (1)

static inline void __enable_cursor_txt(uint8_t c_start, uint8_t c_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | c_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | c_end);
}

static inline void __disable_cursor_txt(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

static inline void __update_cursor_txt(int x, int y)
{
    uint16_t pos = y * VGA_TEXT_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

/**
 * @brief With this code, you get: pos = y * VGA_TEXT_WIDTH + x.
 * To obtain the coordinates, just calculate: y = pos / VGA_TEXT_WIDTH; x = pos % VGA_TEXT_WIDTH;
 */
static inline uint16_t __get_cursor_position_txt(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

#endif // IO_VGA_X86_VGA_H

#include "x86-vga.h"

void __vga_x86_enable_cursor_txt(uint8_t c_start, uint8_t c_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | c_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | c_end);
}

void __vga_x86_disable_cursor_txt(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void __vga_x86_update_cursor_txt(int x, int y)
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
uint16_t __vga_x86_get_cursor_position_txt(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

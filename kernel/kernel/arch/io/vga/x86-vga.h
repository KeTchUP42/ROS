#ifndef IO_VGA_X86_VGA_H
#define IO_VGA_X86_VGA_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include <kernel/arch/io/ports/x86-ports.h>
#include <stdint.h>

#define VGA_TEXT            ((void*)(0xB8000))
#define VGA_TEXT_WIDTH      80
#define VGA_TEXT_HEIGHT     25
#define VGA_TEXT_COLOR_TYPE uint8_t
#define VGA_TEXT_CELL_TYPE  uint16_t
#define VGA_TEXT_CURSOR     0x01

#if !VGA_TEXT_CURSOR
#warning "SYSTEM DO NOT HAVE CURSOR SUPPORT!"
#endif

static inline void vga_enable_cursor_txt(uint8_t c_start, uint8_t c_end)
{
#if VGA_TEXT_CURSOR
    void __vga_x86_enable_cursor_txt(uint8_t c_start, uint8_t c_end);
    __vga_x86_enable_cursor_txt(c_start, c_end);
#endif
}

static inline void vga_disable_cursor_txt(void)
{
#if VGA_TEXT_CURSOR
    void __vga_x86_disable_cursor_txt(void);
    __vga_x86_disable_cursor_txt();
#endif
}

static inline void vga_update_cursor_txt(int x, int y)
{
#if VGA_TEXT_CURSOR
    void __vga_x86_update_cursor_txt(int x, int y);
    __vga_x86_update_cursor_txt(x, y);
#endif
}

static inline uint16_t vga_get_cursor_position_txt(void)
{
#if VGA_TEXT_CURSOR
    uint16_t __vga_x86_get_cursor_position_txt(void);
    return __vga_x86_get_cursor_position_txt();
#else
    return 0;
#endif
}

#endif // IO_VGA_X86_VGA_H

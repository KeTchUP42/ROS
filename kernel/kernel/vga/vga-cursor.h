#ifndef VGA_CURSOR
#define VGA_CURSOR

#if !VGA_TEXT_CURSOR
#warning "SYSTEM DO NOT HAVE CURSOR SUPPORT!"
#endif

static inline void vga_enable_cursor_txt(uint8_t c_start, uint8_t c_end)
{
#if VGA_TEXT_CURSOR
    __enable_cursor_txt(c_start, c_end);
#endif
}

static inline void vga_disable_cursor_txt(void)
{
#if VGA_TEXT_CURSOR
    __disable_cursor_txt();
#endif
}

static inline void vga_update_cursor_txt(int x, int y)
{
#if VGA_TEXT_CURSOR
    __update_cursor_txt(x, y);
#endif
}

static inline uint16_t vga_get_cursor_position_txt(void)
{
#if VGA_TEXT_CURSOR
    return __get_cursor_position_txt();
#else
    return 0;
#endif
}

#endif // VGA_CURSOR

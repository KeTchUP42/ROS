#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble!"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler!"
#endif


/* Hardware text mode color constants. */
enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t tty_row;
size_t tty_column;
uint8_t tty_color;
uint16_t *tty_buffer;

void tty_init(void)
{
    tty_row = 0;
    tty_column = 0;
    tty_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    tty_buffer = (uint16_t *) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; ++y)
    {
        for (size_t x = 0; x < VGA_WIDTH; ++x)
        {
            const size_t index = y * VGA_WIDTH + x;
            tty_buffer[index] = vga_entry(' ', tty_color);
        }
    }
}

void tty_setcolor(uint8_t color)
{
    tty_color = color;
}

void tty_put(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    tty_buffer[index] = vga_entry(c, color);
}

int abs(int n)
{
    if (n < 0)
        return -n;
    return n;
}

void tty_move_cursor_forward(void)
{
    if (++tty_column == VGA_WIDTH)
    {
        tty_column = 0;
        if (++tty_row == VGA_HEIGHT)
            tty_row = 0;
    }
}

void tty_move_cursor_backward(void)
{
    if (tty_column == 0)
    {
        tty_column = VGA_WIDTH - 1;

        if (tty_row == 0)
        {
            tty_row = VGA_HEIGHT - 1;
            return;
        }

        --tty_row;
        return;
    }
    --tty_column;
}

void tty_move_cursor(int n)
{
    void (*tty_move)(void) = (n > 0) ? &tty_move_cursor_forward : &tty_move_cursor_backward;
    n = abs(n);

    while (n-- != 0)
    {
        tty_move();
    }
}

void tty_cursor_n(void)
{
    tty_column = 0;
    if (++tty_row == VGA_HEIGHT)
        tty_row = 0;
}


int tty_escape(char c)
{
    switch (c)
    {
        case '\n':
            tty_cursor_n();
            break;

        default:
            return -1;
    }
    return 0;
}

void tty_putchar(char c)
{
    int code = tty_escape(c);
    if (code != -1) return;

    tty_put(c, tty_color, tty_column, tty_row);
    tty_move_cursor(1);
}

void tty_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        tty_putchar(data[i]);
}

void tty_print(const char *data)
{
    tty_write(data, strlen(data));
}

#include <kernel/dctors.h>

void bye(void)
{
    tty_print("Bye!\n");
}
DTOR(bye)

void kmain(void)
{
    /* Initialize tty interface */
    tty_init();

    /* Newline support is left as an exercise. */
    tty_print("Hello, kernel World!\n");
}

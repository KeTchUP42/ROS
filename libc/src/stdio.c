#include <libc/stdio.h>

int puts(const char *str)
{
#if KERNEL_BUILD
    // TODO: change after tty system update
    void tty_print(const char *data);
    tty_print(str);
    return WRITE_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
}

int putchar(int ch)
{
#if KERNEL_BUILD
    // TODO: change after tty system update
    void tty_putchar(char c);
    tty_putchar(ch);
    return WRITE_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
    return ch;
}

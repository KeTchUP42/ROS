#include <libc/stdio.h>

int puts(const char *str)
{
#ifdef KERNEL_CODE
    tty_print(str);
    return WRITE_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
}

int putchar(int ch)
{
#ifdef KERNEL_CODE
    tty_putchar(ch);
    return WRITE_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
    return ch;
}

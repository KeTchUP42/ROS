#include <libc/stdio.h>

#ifdef KERNEL_CODE
#include <kernel/tty/ktty.h>
#endif

int puts(const char *str)
{
#ifdef KERNEL_CODE
    tty_print(kernel_default_tty(), str);
    return WRITE_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
}

int putchar(int ch)
{
#ifdef KERNEL_CODE
    tty_putchar(kernel_default_tty(), ch);
    return WRITE_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
    return ch;
}

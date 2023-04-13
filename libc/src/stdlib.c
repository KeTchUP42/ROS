#include <libc/stdlib.h>

#include <libc/stdio.h>

void abort(void)
{
#ifdef KERNEL_CODE
    puts("KERNEL PANIC!\n\04");
    __asm__("hlt");
#else
    // TODO: exit() syscall
#endif
    puts("PROGRAM ABORT! INFINITE LOOP ENTERED!");
    while (1);
}

void die(const char *str)
{
    puts(str);
    abort();
}

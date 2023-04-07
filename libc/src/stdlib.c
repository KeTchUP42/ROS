#include <libc/stdlib.h>
#include <libc/stdio.h>

void abort(void)
{
#if (KERNEL_BUILD == 1)
    puts("KERNEL PANIC!\n");
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

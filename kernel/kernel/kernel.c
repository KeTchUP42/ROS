#if defined(__linux__)
#error "YOU ARE NOT USING A CROSS-COMPILER, YOU WILL MOST CERTAINLY RUN INTO TROUBLE!"
#endif

#include <libc/stdio.h>

void kmain(void)
{
    puts("\etHELLO KERNEL WORLD!\04\n");
}

#include <kernel/dctors.h>
#include <libc/stdio.h>

static void bye(void)
{
    puts("Bye!\n");
}

DTOR(bye, 999)

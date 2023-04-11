#include <kernel/dctors.h>
#include <kernel/tty/tty.h>

#include <libc/stdio.h>

static void tty_fini(void)
{
    tty_dealloc_all();
}

static void bye(void)
{
    puts("Bye!\04\n");
}

DTOR(bye, 999)
DTOR(tty_fini, 1)

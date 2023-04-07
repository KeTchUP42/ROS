#include <kernel/dctors.h>
#include <libc/stdio.h>

void tty_init(void);

static void hi(void)
{
    puts("Hi!\n");
}

/* Initialize tty interface */
CTOR(tty_init, 999)
/* init gap! */
CTOR(hi, 1)

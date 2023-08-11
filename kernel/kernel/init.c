#include <kernel/dctors.h>
#include <kernel/tty/ktty.h>

#include <libc/stdio.h>

static void tty_init(void)
{
    vga_disable_cursor_txt();
    tty_add(kernel_default_tty());
    puts("TTY system configured successful...\n\04");
}

static void hi(void)
{
    puts("Hi!\n\04");
}

/* Initialize tty interface */
CTOR(tty_init, 999)
/* init gap! */
CTOR(hi, 1)

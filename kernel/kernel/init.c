#include <kernel/dctors.h>

void tty_init(void);

static void hi(void)
{
    void tty_print(const char *data);
    tty_print("Hi!\n");
}

CTOR(tty_init, 999)
/* init gap! */
CTOR(hi, 1)

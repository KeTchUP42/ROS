#include <kernel/dctors.h>

static void bye(void)
{
    void tty_print(const char *data);
    tty_print("Bye!\n");
}

DTOR(bye, 999)

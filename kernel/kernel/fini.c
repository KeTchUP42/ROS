#include <kernel/dctors.h>

#include <kernel/tty/tty.h>

#include <kernel/memory/sysmmap.h>
#include <kernel/tty/ktty.h>
#include <kernel/memory/kheap.h>

#include <libc/stdio.h>

static void bye(void)
{
    puts("Bye!\n\04");
}

static void mem_check(void)
{
    size_t count = kheap_nodes_count();
    if (count != 0)
    {
        kernel_default_tty_reset(NULL);
        tty_add(kernel_default_tty());
        printf("mem_check(): kernel heap memory leak, %u heap nodes left!\n\04", (unsigned int)count);
        tty_dealloc_all();
    }
}

DTOR(bye, 999)
DTOR(sysmmap_dealloc_all, 998)
/* fini gap! */
DTOR(tty_dealloc_all, 2)
DTOR(mem_check, 1)

#if defined(__linux__)
#error "YOU ARE NOT USING A CROSS-COMPILER, YOU WILL MOST CERTAINLY RUN INTO TROUBLE!"
#endif

#include <kernel/memory/sysmmap.h>

#include <stddef.h>
#include <stdint.h>

#include <libc/stdio.h>
#include <libc/time.h>

#include <kernel/memory/kheap.h>


#include <kernel/arch/io/ps-2/keyboard/keyboard.h>

void kmain(multiboot_info_t *mmap, unsigned int magic)
{
    sysmmap_init(mmap, magic);
    printf("%d %d %d\n", kheap_nodes_count(), kheap_allocated(), kheap_left());

    while (1)
    {
        puts("\04");
        __asm__("hlt");
    }
}

#if defined(__linux__)
#error "YOU ARE NOT USING A CROSS-COMPILER, YOU WILL MOST CERTAINLY RUN INTO TROUBLE!"
#endif

#include <kernel/memory/sysmmap.h>

#include <stddef.h>
#include <stdint.h>

#include <libc/stdio.h>

#include <kernel/memory/kheap.h>

void kmain(multiboot_info_t *mmap, unsigned int magic)
{
    sysmmap_init(mmap, magic);
    printf("%d %d %d\n", kheap_nodes_count(), kheap_allocated(), kheap_left());
}

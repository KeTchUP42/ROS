#ifndef SYSMMAP_H
#define SYSMMAP_H

#include <kernel/arch/boot/multiboot.h>

typedef multiboot_memory_map_t *multiboot_memory_map_ptr_t;

typedef struct mmap_array
{
    multiboot_memory_map_ptr_t *mmaps;
    multiboot_uint32_t mmaps_type;
    multiboot_uint32_t mmaps_count;
} mmap_array_type;

void sysmmap_init(multiboot_info_t *mbd, unsigned int magic);

const mmap_array_type *sysmmap_avaiable(void);
const mmap_array_type *sysmmap_reserved(void);
const mmap_array_type *sysmmap_acpi_reclaimable(void);
const mmap_array_type *sysmmap_nvs(void);
const mmap_array_type *sysmmap_badram(void);

void sysmmap_dealloc_all(void);

#endif // SYSMMAP_H

#include <kernel/memory/sysmmap.h>

#include <kernel/memory/kheap.h>
#include <libc/stdlib.h>

#include <stddef.h>

static mmap_array_type mmap_array_avaiable = { NULL, MULTIBOOT_MEMORY_AVAILABLE, 0 };
static mmap_array_type mmap_array_reserved = { NULL, MULTIBOOT_MEMORY_RESERVED, 0 };
static mmap_array_type mmap_array_acpi_reclaimable = { NULL, MULTIBOOT_MEMORY_ACPI_RECLAIMABLE, 0 };
static mmap_array_type mmap_array_nvs = { NULL, MULTIBOOT_MEMORY_NVS, 0 };
static mmap_array_type mmap_array_badram = { NULL, MULTIBOOT_MEMORY_BADRAM, 0 };

void sysmmap_init(multiboot_info_t *mbd, unsigned int magic)
{
    /* Make sure the magic number matches for memory mapping */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        die("sysmmap_init(): invalid magic number given by kernel bootloader!\n");
    }

    /* Check bit 6 to see if we have a valid memory map */
    if (!(mbd->flags >> 6 & 0x1))
    {
        die("sysmmap_init(): invalid memory map given by kernel bootloader!\n");
    }

    size_t multiboot_mmap_mem_lenght = mbd->mmap_length;
    size_t universal_mmap_array_sz = sizeof(multiboot_memory_map_ptr_t) *
                                     (multiboot_mmap_mem_lenght / sizeof(multiboot_memory_map_t));

    /* Loop through the memory map and store the values */
    for (size_t i = 0; i < multiboot_mmap_mem_lenght; i += sizeof(multiboot_memory_map_t))
    {

        multiboot_memory_map_t *mmmt = (multiboot_memory_map_t *)((ptrdiff_t)mbd->mmap_addr + i);

        switch (mmmt->type)
        {
            case MULTIBOOT_MEMORY_AVAILABLE:
            {
                if (mmap_array_avaiable.mmaps == NULL)
                    mmap_array_avaiable.mmaps = kmalloc(universal_mmap_array_sz);

                *(mmap_array_avaiable.mmaps + mmap_array_avaiable.mmaps_count) = mmmt;
                ++mmap_array_avaiable.mmaps_count;
            }
            break;

            case MULTIBOOT_MEMORY_RESERVED:
            {
                if (mmap_array_reserved.mmaps == NULL)
                    mmap_array_reserved.mmaps = kmalloc(universal_mmap_array_sz);

                *(mmap_array_reserved.mmaps + mmap_array_reserved.mmaps_count) = mmmt;
                ++mmap_array_reserved.mmaps_count;
            }
            break;

            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            {
                if (mmap_array_acpi_reclaimable.mmaps == NULL)
                    mmap_array_acpi_reclaimable.mmaps = kmalloc(universal_mmap_array_sz);

                *(mmap_array_acpi_reclaimable.mmaps + mmap_array_acpi_reclaimable.mmaps_count) = mmmt;
                ++mmap_array_acpi_reclaimable.mmaps_count;
            }
            break;

            case MULTIBOOT_MEMORY_NVS:
            {

                if (mmap_array_nvs.mmaps == NULL)
                    mmap_array_nvs.mmaps = kmalloc(universal_mmap_array_sz);

                *(mmap_array_nvs.mmaps + mmap_array_nvs.mmaps_count) = mmmt;
                ++mmap_array_nvs.mmaps_count;
            }
            break;

            case MULTIBOOT_MEMORY_BADRAM:
            {
                if (mmap_array_badram.mmaps == NULL)
                    mmap_array_badram.mmaps = kmalloc(universal_mmap_array_sz);

                *(mmap_array_badram.mmaps + mmap_array_badram.mmaps_count) = mmmt;
                ++mmap_array_badram.mmaps_count;
            }
            break;
            default:
                die("sysmmap_init(): unknown multiboot memory map type!\n");
        };
    }
}

const mmap_array_type *sysmmap_avaiable(void)
{
    return &mmap_array_avaiable;
}
const mmap_array_type *sysmmap_reserved(void)
{
    return &mmap_array_reserved;
}
const mmap_array_type *sysmmap_acpi_reclaimable(void)
{
    return &mmap_array_acpi_reclaimable;
}
const mmap_array_type *sysmmap_nvs(void)
{
    return &mmap_array_nvs;
}
const mmap_array_type *sysmmap_badram(void)
{
    return &mmap_array_badram;
}

void sysmmap_dealloc_all(void)
{
    kfree(mmap_array_avaiable.mmaps);
    kfree(mmap_array_reserved.mmaps);
    kfree(mmap_array_acpi_reclaimable.mmaps);
    kfree(mmap_array_nvs.mmaps);
    kfree(mmap_array_badram.mmaps);
}

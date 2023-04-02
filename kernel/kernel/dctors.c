#include <kernel/dctors.h>

void kernel_init(void)
{
    uintptr_t init_array = (uintptr_t)__init_array_start;
    uintptr_t init_array_end = (uintptr_t)__init_array_end;

    while (init_array != init_array_end)
    {
        (*((ctor_type *)init_array))();
        init_array += sizeof(ctor_type);
    }
}

void kernel_fini(void)
{
    uintptr_t fini_array = (uintptr_t)__fini_array_start;
    uintptr_t fini_array_end = (uintptr_t)__fini_array_end;

    while (fini_array != fini_array_end)
    {
        (*((dtor_type *)fini_array))();
        fini_array += sizeof(dtor_type);
    }
}

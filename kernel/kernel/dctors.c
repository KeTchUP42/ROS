#include <kernel/dctors.h>

void kernel_init(void)
{
    ctor_type *init_array = (ctor_type *)__init_array_start;
    ctor_type *init_array_end = (ctor_type *)__init_array_end;

    while (init_array != init_array_end)
    {
        (*init_array++)();
    }
}

void kernel_fini(void)
{
    dtor_type *fini_array = (dtor_type *)__fini_array_start;
    dtor_type *fini_array_end = (dtor_type *)__fini_array_end;

    while (fini_array != fini_array_end)
    {
        (*fini_array++)();
    }
}

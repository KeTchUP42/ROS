#include <libc/time.h>

#ifdef KERNEL_CODE
#include <kernel/arch/cpu/clock/time.h>
#endif

void sleep(size_t mls)
{
#ifdef KERNEL_CODE
    for (size_t i = 0; i < mls; ++i)
        for (size_t j = 0; j < WAIT_FOR_MILLISECOND; ++j)
            cpu_wait();
#else
    // TODO: sleep() syscall
#endif
}

#ifndef CPU_INTS_X86_IHARD_H
#define CPU_INTS_X86_IHARD_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include "x86-ints-cfg.h"

void ihard_sys_init(void);

void __int_handler_wrapper(uint32_t, cpu_int_callback_t);

#endif // CPU_INTS_X86_IHARD_H

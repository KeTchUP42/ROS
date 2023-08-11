#ifndef CPU_INTS_X86_IHARD_H
#define CPU_INTS_X86_IHARD_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include "x86-ints-cfg.h"
#include "x86-pic.h"

static inline void ihard_sys_init(void)
{
#if PIC_INTS_SYS
    pic_irq_set_mask(0);
    pic_irq_clear_mask(1);
    pic_irq_clear_mask(2);
    pic_irq_set_mask(3);
    pic_irq_set_mask(4);
    pic_irq_set_mask(5);
    pic_irq_set_mask(6);
    pic_irq_clear_mask(7);
    pic_irq_set_mask(8);
    pic_irq_set_mask(9);
    pic_irq_set_mask(10);
    pic_irq_set_mask(11);
    pic_irq_set_mask(12);
    pic_irq_set_mask(13);
    pic_irq_set_mask(14);
    pic_irq_clear_mask(15);
    pic_enable();
#endif
}

void __int_handler_wrapper(uint32_t, cpu_int_callback_t);

#endif // CPU_INTS_X86_IHARD_H

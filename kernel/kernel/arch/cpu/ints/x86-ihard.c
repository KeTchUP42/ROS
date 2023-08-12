#include "x86-ihard.h"

#include <libc/stdio.h>

#include "x86-pic.h"

void ihard_sys_init(void)
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

#define PIC_INT2PIC( INT_NUM ) (INT_NUM - PIC_INTS_OFFSET)

void __int_handler_wrapper(uint32_t inum, cpu_int_callback_t cpu_int_handler)
{
#if PIC_INTS_SYS
    bool is_pic = (inum >= PIC_INTS_OFFSET) && (inum <= PIC_INTS_OFFSET + PIC_INTS_COUNT - 1);

    if (is_pic)
    {
        if (PIC_INT2PIC(inum) == 7)
        {
            uint16_t isr = pic_get_isr();
            if (!(isr & 0x80))
                return;
        }
        if (PIC_INT2PIC(inum) == 15)
        {
            uint16_t isr = pic_get_isr();
            if (!(isr & 0x8000))
            {
                /* Sending EOI to master PIC: */
                pic_send_eoi(0);
                return;
            }
        }
        pic_send_eoi(PIC_INT2PIC(inum));
    }
#endif
    cpu_int_handler(inum);
}

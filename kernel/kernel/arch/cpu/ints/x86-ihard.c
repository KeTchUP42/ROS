#include "x86-ihard.h"

#include <libc/stdio.h>

#define PIC_INT2PIC( INT_NUM ) (INT_NUM - PIC_INTS_OFFSET)

void __int_handler_wrapper(uint32_t inum, cpu_int_callback_t cpu_int_handler)
{
#if PIC_INTS_SYS
    bool is_pic = (inum >= PIC_INTS_OFFSET) && (inum <= PIC_INTS_OFFSET + PIC_INTS_COUNT + 1);

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

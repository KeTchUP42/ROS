#include "x86-pic.h"

#include <kernel/arch/io/ports/x86-ports.h>
#include <kernel/arch/cpu/clock/time.h>

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC_SLV_COMMAND, PIC_EOI);

    outb(PIC_MST_COMMAND, PIC_EOI);
}

#if 0
void pic_send_eoi_all(void)
{
    outb(PIC_SLV_COMMAND, PIC_EOI);
    outb(PIC_MST_COMMAND, PIC_EOI);
}
#endif

void pic_remap_offsets(uint8_t offset1, uint8_t offset2)
{
    uint8_t master_mask;
    uint8_t slave_mask;

    master_mask = inb(PIC_MST_DATA);
    slave_mask = inb(PIC_SLV_DATA);
    /* Starts the initialization sequence (in cascade mode) */
    outb(PIC_MST_COMMAND, ICW1_INIT | ICW1_ICW4);
    cpu_wait();
    outb(PIC_SLV_COMMAND, ICW1_INIT | ICW1_ICW4);
    cpu_wait();
    outb(PIC_MST_DATA, offset1);    /* ICW2: Master PIC vector offset */
    cpu_wait();
    outb(PIC_SLV_DATA, offset2);    /* ICW2: Slave PIC vector offset */
    cpu_wait();
    /* ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100) */
    outb(PIC_MST_DATA, 4);
    cpu_wait();
    /* ICW3: tell Slave PIC its cascade identity (0000 0010) */
    outb(PIC_SLV_DATA, 2);
    cpu_wait();
    /* ICW4: have the PICs use 8086 mode (and not 8080 mode) */
    outb(PIC_MST_DATA, ICW4_8086);
    cpu_wait();
    outb(PIC_SLV_DATA, ICW4_8086);
    cpu_wait();

    outb(PIC_MST_DATA, master_mask);
    outb(PIC_SLV_DATA, slave_mask);
}

void pic_disable(void)
{
    outb(PIC_MST_DATA, 0xFF);
    outb(PIC_SLV_DATA, 0xFF);
}

void pic_irq_set_mask(uint8_t irqline)
{
    uint16_t port;
    uint8_t value;

    if (irqline < 8)
    {
        port = PIC_MST_DATA;
    }
    else
    {
        port = PIC_SLV_DATA;
        irqline -= 8;
    }
    value = inb(port) | (1 << irqline);
    outb(port, value);
}

void pic_irq_clear_mask(uint8_t irqline)
{
    uint16_t port;
    uint8_t value;

    if (irqline < 8)
    {
        port = PIC_MST_DATA;
    }
    else
    {
        port = PIC_SLV_DATA;
        irqline -= 8;
    }
    value = inb(port) & ~(1 << irqline);
    outb(port, value);
}

uint16_t __pic_get_irq_reg(uint8_t ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC_MST_COMMAND, ocw3);
    outb(PIC_SLV_COMMAND, ocw3);
    return (inb(PIC_SLV_COMMAND) << 8) | inb(PIC_MST_COMMAND);
}

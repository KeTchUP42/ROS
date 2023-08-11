#ifndef CPU_INTS_X86_PIC_H
#define CPU_INTS_X86_PIC_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include <kernel/arch/io/ports/x86-ports.h>
#include <kernel/arch/cpu/clock/time.h>

#include <stdint.h>

#define PIC_INTS_SYS       0x01        /* Indicates that kernel uses old-style PIC chip */
#define PIC_INTS_OFFSET    0x20
#define PIC_INTS_COUNT     0x10

#define PIC_MST            0x20        /* IO base address for master PIC */
#define PIC_SLV            0xA0        /* IO base address for slave PIC */
#define PIC_MST_COMMAND    PIC_MST
#define PIC_MST_DATA      (PIC_MST + 1)
#define PIC_SLV_COMMAND    PIC_SLV
#define PIC_SLV_DATA      (PIC_SLV + 1)

#define PIC_EOI            0x20        /* End-of-interrupt command code */

static inline void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC_SLV_COMMAND, PIC_EOI);

    outb(PIC_MST_COMMAND, PIC_EOI);
}

#if 0
static inline void pic_send_eoi_all(void)
{
    outb(PIC_SLV_COMMAND, PIC_EOI);
    outb(PIC_MST_COMMAND, PIC_EOI);
}
#endif

#define ICW1_ICW4       0x01        /* Indicates that ICW4 will be present */
#define ICW1_SINGLE     0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08        /* Level triggered (edge) mode */
#define ICW1_INIT       0x10        /* Initialization - required! */

#define ICW4_8086       0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C        /* Buffered mode/master */
#define ICW4_SFNM       0x10        /* Special fully nested (not) */

/**
 * @brief Re-inits pic chips with new offsets:
 *
 * @param offset1 - vector offset for master PIC
 *      vectors on the master become offset1..offset1+7
 * @param offset2 - same for slave PIC: offset2..offset2+7
 */
static inline void pic_remap_offsets(uint8_t offset1, uint8_t offset2)
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

/**
 * @brief Inits and remaps PIC offsets to useable values:
 */
static inline void pic_enable(void)
{
    /* In i386 protected mode first 32 ints and reserved for system exceptions: */
    pic_remap_offsets(PIC_INTS_OFFSET, PIC_INTS_OFFSET + 8);
}

/**
 * @brief If you are going to use the processor local APIC and the IOAPIC, you must first disable the PIC:
 */
static inline void pic_disable(void)
{
    outb(PIC_MST_DATA, 0xFF);
    outb(PIC_SLV_DATA, 0xFF);
}

/**
 * @brief Sets needed irq line mask-register bit:
 *
 * @param irqline Number of irq line [0-15]
 */
static inline void pic_irq_set_mask(uint8_t irqline)
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

/**
 * @brief Clears needed irq line mask-register bit:
 *
 * @param irqline Number of irq line [0-15]
 */
static inline void pic_irq_clear_mask(uint8_t irqline)
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


#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */

/* Helper func */
static uint16_t __pic_get_irq_reg(uint8_t ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC_MST_COMMAND, ocw3);
    outb(PIC_SLV_COMMAND, ocw3);
    return (inb(PIC_SLV_COMMAND) << 8) | inb(PIC_MST_COMMAND);
}

/**
 * @brief Returns the combined value of the cascaded PICs irq request register.
 */
static inline uint16_t pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}

/**
 * @brief Returns the combined value of the cascaded PICs in-service register.
 */
static inline uint16_t pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}

#endif // CPU_INTS_X86_PIC_H

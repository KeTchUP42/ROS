#ifndef CPU_INTS_X86_PIC_H
#define CPU_INTS_X86_PIC_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

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

void pic_send_eoi(uint8_t irq);

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
void pic_remap_offsets(uint8_t offset1, uint8_t offset2);

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
void pic_disable(void);

/**
 * @brief Sets needed irq line mask-register bit:
 *
 * @param irqline Number of irq line [0-15]
 */
void pic_irq_set_mask(uint8_t irqline);

/**
 * @brief Clears needed irq line mask-register bit:
 *
 * @param irqline Number of irq line [0-15]
 */
void pic_irq_clear_mask(uint8_t irqline);

#define PIC_READ_IRR    0x0A  /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR    0x0B  /* OCW3 irq service next CMD read */

/**
 * @brief Returns the combined value of the cascaded PICs irq request register.
 */
static inline uint16_t pic_get_irr(void)
{
    uint16_t __pic_get_irq_reg(uint8_t ocw3);
    return __pic_get_irq_reg(PIC_READ_IRR);
}

/**
 * @brief Returns the combined value of the cascaded PICs in-service register.
 */
static inline uint16_t pic_get_isr(void)
{
    uint16_t __pic_get_irq_reg(uint8_t ocw3);
    return __pic_get_irq_reg(PIC_READ_ISR);
}

#endif // CPU_INTS_X86_PIC_H

#include "x86-setup.h"

/*----------------------------------------------------------------------------*/
/*------------------------------------GDTR------------------------------------*/
/*----------------------------------------------------------------------------*/

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_type;

/**
 * @brief Intermediate register storage in memory:
 */
static gdtr_type GDTR;

static void set_gdtr(uint16_t limit, uint32_t base)
{
    GDTR.limit = limit;
    GDTR.base = base;
    __asm__ ( "lgdt %0" : : "m"(GDTR) );
}

/*----------------------------------------------------------------------------*/
/*------------------------------------TSS-------------------------------------*/
/*----------------------------------------------------------------------------*/

#include <libc/string.h>

typedef struct
{
    uint32_t prev_tss;
    uint32_t esp0;     /* The stack pointer to load when changing to kernel mode. */
    uint32_t ss0;      /* The stack segment to load when changing to kernel mode. */
    /* EVERYTHING BELOW HERE IS UNUSED: */
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((__packed__)) tss_entry_type;

static tss_entry_type TSS;

static void tss_setup(void)
{
    /* Ensure the TSS is initially zero'd: */
    memset(&TSS, 0, sizeof(tss_entry_type));

    TSS.ss0  = GDT_OFF_KERNEL_DATA_SEGMENT;
    /* THIS VALUE WILL BE SETUP BEFORE INIT PROC START: */
    TSS.esp0 = 0x0;
}

void __save_kernel_stack(void *esp)
{
    TSS.esp0 = (uint32_t)((ptrdiff_t)esp);
}

static void tss_flush(uint16_t n)
{
    __asm__(
        ".intel_syntax noprefix   \n"
        "   mov ax, %0            \n"
        "   ltr ax                \n"
        ".att_syntax              \n"
        :: "r"(n)
    );
}

/*----------------------------------------------------------------------------*/
/*------------------------------------GDT-------------------------------------*/
/*----------------------------------------------------------------------------*/

typedef uint64_t gdt_des_type;

/**
 * 0x0000 - Null Descriptor
 * 0x0008 - Kernel Mode Code Segment
 * 0x0010 - Kernel Mode Data Segment
 * 0x0018 - User Mode Code Segment
 * 0x0020 - User Mode Data Segment
 * 0x0028 - Task State Segment
 */
static gdt_des_type GDT[6];

static gdt_des_type gdt_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    gdt_des_type dptr;

    /* Create the high 32 bit segment: */
    dptr  =  limit       & 0x000F0000;   /* set limit bits 19:16 */
    dptr |= (flag <<  8) & 0x00F0FF00;   /* set type, p, dpl, s, g, d/b, l and avl fields */
    dptr |= (base >> 16) & 0x000000FF;   /* set base bits 23:16  */
    dptr |=  base        & 0xFF000000;   /* set base bits 31:24  */

    /* Shift by 32 to allow for low part of segment: */
    dptr <<= 32;

    /* Create the low 32 bit segment:  */
    dptr |= base  << 16;                 /* set base bits 15:0   */
    dptr |= limit  & 0x0000FFFF;         /* set limit bits 15:0  */

    return dptr;
}

static void __i386_reload_segments(void)
{
    __asm__(
        ".intel_syntax noprefix   \n"
        "   jmp   0x08:.reload_cs \n"
        ".reload_cs:              \n"
        "   mov   ax, 0x10        \n"
        "   mov   ds, ax          \n"
        "   mov   es, ax          \n"
        "   mov   fs, ax          \n"
        "   mov   gs, ax          \n"
        "   mov   ss, ax          \n"
        ".att_syntax              \n"
    );
}

static void gdt_setup(void)
{
    GDT[0] = gdt_create_descriptor(0, 0, 0);
    GDT[1] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    GDT[2] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    GDT[3] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    GDT[4] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
    // TODO: USE LINUX LIKE MACRO HERE:
    // GDT AND GDTR MUST CONTAIN PHYSICAL ADDRESS:
    GDT[5] = gdt_create_descriptor((uint32_t)((ptrdiff_t)&TSS), sizeof(tss_entry_type), GDT_TSS_PL0);
    set_gdtr(sizeof(GDT), (uint32_t)((ptrdiff_t)&GDT));

    tss_setup();
    tss_flush(GDT_OFF_TSS_SEGMENT);

    __i386_reload_segments();
}

/*----------------------------------------------------------------------------*/
/*------------------------------------IDT-------------------------------------*/
/*----------------------------------------------------------------------------*/

#include <kernel/arch/cpu/ints/ints-cfg.h>

typedef struct
{
    uint16_t    limit;
    uint32_t    base;
} __attribute__((packed)) idtr_type;

static idtr_type IDTR;

static void set_idtr(uint16_t limit, uint32_t base)
{
    IDTR.limit = limit;
    IDTR.base = base;
    __asm__ ("lidt %0" : : "m"(IDTR));
}

typedef struct
{
    uint16_t    isr_low;      /* The lower 16 bits of the ISR's address */
    uint16_t    kernel_cs;    /* The GDT segment selector that the CPU will load into CS before calling the ISR */
    uint8_t     reserved;     /* Set to zero */
    uint8_t     attributes;   /* Type and attributes; see the IDT page */
    uint16_t    isr_high;     /* The higher 16 bits of the ISR's address */
} __attribute__((packed)) idt_entry_type;

__attribute__((aligned(0x10)))
static idt_entry_type IDT[CPU_INTS_VECTOR_SIZE];

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    idt_entry_type *dptr = &IDT[vector];
    dptr->isr_low        = (uint32_t)((ptrdiff_t)isr) & 0xFFFF;
    dptr->kernel_cs      = GDT_OFF_KERNEL_CODE_SEGMENT;
    dptr->attributes     = flags;
    dptr->isr_high       = (uint32_t)((ptrdiff_t)isr) >> 16;
    dptr->reserved       = 0;
}

static void idt_setup(void)
{
    for (size_t vec = 0; vec < CPU_INTS_VECTOR_SIZE; ++vec)
    {
        if (GET_INT_HANDLER(vec) != 0)
            idt_set_descriptor(vec, GET_INT_HANDLER(vec), 0x8e);
    }

    set_idtr(sizeof(IDT) - 1, (uint32_t)((ptrdiff_t)&IDT[0]));
}

/*----------------------------------------------------------------------------*/

void __i386_cpu_setup(void)
{
    __asm__ ("cli");

    /* FULL CPU SETUP: */
    gdt_setup();
    idt_setup();

    /* Initialize interrupts system */
    ihard_sys_init();

    __asm__ ("sti");
}

#ifndef CPU_MODE_X86_USER_H
#define CPU_MODE_X86_USER_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include <kernel/arch/boot/x86/x86-setup.h>

static inline void user_mode_jump(void *code, void *esp, uint64_t flags)
{
    uint32_t eflags = flags | 0xFFFFFFFF;
    uint16_t usr_data_seg = GDT_OFF_USER_DATA_SEGMENT | 3;
    uint16_t usr_code_seg = GDT_OFF_USER_CODE_SEGMENT | 3;

    __asm__(
        ".intel_syntax noprefix   \n"
        "   mov ax, %0            \n"
        "   mov ds, ax            \n"
        "   mov es, ax            \n"
        "   mov fs, ax            \n"
        "   mov gs, ax            \n"
        ".att_syntax              \n"
        :: "r"(usr_data_seg)
    );

    uint32_t kernel_esp;
    __asm__(
        ".intel_syntax noprefix   \n"
        "   mov %0, esp           \n"
        ".att_syntax              \n"
        : "=r"(kernel_esp)
    );

    __save_kernel_stack((void *)kernel_esp);

    uint32_t usr_data_seg32 = usr_data_seg;
    uint32_t usr_code_seg32 = usr_code_seg;

    __asm__(
        ".intel_syntax noprefix   \n"
        "   push %4               \n"
        "   push %1               \n"
        "   push %2               \n"
        "   push %3               \n"
        "   push %0               \n"
        "   iret                  \n"
        ".att_syntax              \n"
        :: "r"(code), "r"(esp), "r"(eflags), "r"(usr_code_seg32), "r"(usr_data_seg32)
    );
}

#endif // CPU_MODE_X86_USER_H

#ifndef IO_PS2_X86_KEYBOARD_H
#define IO_PS2_X86_KEYBOARD_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include <kernel/tty/sys-ksc.h>

#define PS2_X86_KEYBOARD   0x01

static inline sys_kb_scn_cds_type kb_read_ksc(void)
{
    /* Specific driver function. Reads specific scan-codes from from keyboard buffer and
     * converts them to system general scan-codes. Zero code is error flag */
    sys_kb_scn_cds_type __ps2_x86_kb_read_ksc(void);
    return __ps2_x86_kb_read_ksc();
}

static inline int kb_is_key_prssd(enum sys_kb_scn_cds code)
{
    /* 1 - true, 0 - false, -1 - no info */
    int __ps2_x86_kb_is_key_prssd(enum sys_kb_scn_cds);
    return __ps2_x86_kb_is_key_prssd(code);
}

static inline char kb_read_ch(void)
{
    /* Specific driver function. Reads specific char from sequence of keyboard scan-codes. */
    char __ps2_x86_kb_read_ch(void);
    return __ps2_x86_kb_read_ch();
}
#endif // IO_PS2_X86_KEYBOARD_H

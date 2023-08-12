#ifndef IO_PS2_X86_KEYBOARD_H
#define IO_PS2_X86_KEYBOARD_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#define PS2_X86_KEYBOARD   0x01
#define KEYBOARD_NULL_CODE 0x00

/**
 * @brief General driver function.
 */
static inline char kb_read_ch(void)
{
    //TODO:
    /* Specific driver function. Reads specific scan-codes from from keyboard buffer and
     * converts them to system general scan-codes. */
    char __ps2_x86_kb_read_ch(void);
    return __ps2_x86_kb_read_ch();
}

#endif // IO_PS2_X86_KEYBOARD_H

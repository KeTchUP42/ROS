#ifndef IO_PS2_X86_KEYBOARD_H
#define IO_PS2_X86_KEYBOARD_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#define KEYBOARD_NULL_CODE 0x00

/**
 * @brief Reads ASCII char from keyboard buffer, uses in ps-2 keyboard int handler.
 */
char kb_read_ch(void);

#endif // IO_PS2_X86_KEYBOARD_H

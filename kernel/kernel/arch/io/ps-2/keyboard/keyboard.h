#ifndef IO_PS2_KEYBOARD_H
#define IO_PS2_KEYBOARD_H

#ifdef ARCH_X86
#include "x86-keyboard.h"
#else
#error "KERNEL CAN NOT BE BUILD WITH SUCH ARCHITECTURE!"
#endif


#endif // IO_PS2_KEYBOARD_H

#ifndef IO_VGA_VGA_H
#define IO_VGA_VGA_H

#ifdef ARCH_X86
#include "x86-vga.h"
#else
#error "KERNEL CAN NOT BE BUILD WITH SUCH ARCHITECTURE!"
#endif

#endif // IO_VGA_VGA_H

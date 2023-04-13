#ifndef IO_PORTS_X86_PORTS_H
#define IO_PORTS_X86_PORTS_H

#ifndef KERNEL_CODE
#error "DRIVERS ARE ONLY AVAILABLE IN THE KERNEL CODE!"
#endif

#ifndef ARCH_X86
#error "THIS CODE ONLY AVAILABLE ON X86 ARCHITECTURE!"
#endif

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif // IO_PORTS_X86_PORTS_H

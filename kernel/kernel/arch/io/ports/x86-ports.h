#ifndef IO_PORTS_X86_PORTS_H
#define IO_PORTS_X86_PORTS_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

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

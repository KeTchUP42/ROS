#ifndef IHUB_H
#define IHUB_H

#include <kernel/arch/cpu/ints/ints-cfg.h>

#include <libc/stdio.h>

void __exception(uint32_t my_num)
{
    printf("BLYAT: %d!\n\04", my_num);
}

void __nothing(uint32_t my_num)
{

}

#include <kernel/arch/io/ps-2/keyboard/keyboard.h>

void __int_handler(uint32_t my_num)
{
    if (my_num == 33)
    {
        char ch = kb_read_ch();
        if (ch == KEYBOARD_NULL_CODE) return;
        putchar(ch);
        if (ch == '\b')
        {
            putchar(' ');
            putchar('\b');
        }
    }
}

CPU_INT_DEF(0, __exception);
CPU_INT_DEF(1, __exception);
CPU_INT_DEF(2, __exception);
CPU_INT_DEF(3, __exception);
CPU_INT_DEF(4, __exception);
CPU_INT_DEF(5, __exception);
CPU_INT_DEF(6, __exception);
CPU_INT_DEF(7, __exception);
CPU_INT_DEF(8, __exception);
CPU_INT_DEF(9, __exception);
CPU_INT_DEF(10, __exception);
CPU_INT_DEF(11, __exception);
CPU_INT_DEF(12, __exception);
CPU_INT_DEF(13, __exception);
CPU_INT_DEF(14, __exception);
CPU_INT_DEF(15, __exception);
CPU_INT_DEF(16, __exception);
CPU_INT_DEF(17, __exception);
CPU_INT_DEF(18, __exception);
CPU_INT_DEF(19, __exception);
CPU_INT_DEF(20, __exception);
CPU_INT_DEF(21, __exception);
CPU_INT_DEF(22, __exception);
CPU_INT_DEF(23, __exception);
CPU_INT_DEF(24, __exception);
CPU_INT_DEF(25, __exception);
CPU_INT_DEF(26, __exception);
CPU_INT_DEF(27, __exception);
CPU_INT_DEF(28, __exception);
CPU_INT_DEF(29, __exception);
CPU_INT_DEF(30, __exception);
CPU_INT_DEF(31, __exception);

CPU_INT_DEF(32, __nothing);
CPU_INT_DEF(33, __int_handler);
CPU_INT_DEF(34, __nothing);
CPU_INT_DEF(35, __nothing);
CPU_INT_DEF(36, __nothing);
CPU_INT_DEF(37, __nothing);
CPU_INT_DEF(38, __nothing);
CPU_INT_DEF(39, __nothing);
CPU_INT_DEF(40, __nothing);
CPU_INT_DEF(41, __nothing);
CPU_INT_DEF(42, __nothing);
CPU_INT_DEF(43, __nothing);
CPU_INT_DEF(44, __nothing);
CPU_INT_DEF(45, __nothing);
CPU_INT_DEF(46, __nothing);
CPU_INT_DEF(47, __nothing);


INT_VECTOR_START

DEFINE_HANDLER(0)
DEFINE_HANDLER(1)
DEFINE_HANDLER(2)
DEFINE_HANDLER(3)
DEFINE_HANDLER(4)
DEFINE_HANDLER(5)
DEFINE_HANDLER(6)
DEFINE_HANDLER(7)
DEFINE_HANDLER(8)
DEFINE_HANDLER(9)
DEFINE_HANDLER(10)
DEFINE_HANDLER(11)
DEFINE_HANDLER(12)
DEFINE_HANDLER(13)
DEFINE_HANDLER(14)
DEFINE_HANDLER(15)
DEFINE_HANDLER(16)
DEFINE_HANDLER(17)
DEFINE_HANDLER(18)
DEFINE_HANDLER(19)
DEFINE_HANDLER(20)
DEFINE_HANDLER(21)
DEFINE_HANDLER(22)
DEFINE_HANDLER(23)
DEFINE_HANDLER(24)
DEFINE_HANDLER(25)
DEFINE_HANDLER(26)
DEFINE_HANDLER(27)
DEFINE_HANDLER(28)
DEFINE_HANDLER(29)
DEFINE_HANDLER(30)
DEFINE_HANDLER(31)

DEFINE_HANDLER(32)
DEFINE_HANDLER(33)
DEFINE_HANDLER(34)
DEFINE_HANDLER(35)
DEFINE_HANDLER(36)
DEFINE_HANDLER(37)
DEFINE_HANDLER(38)
DEFINE_HANDLER(39)
DEFINE_HANDLER(40)
DEFINE_HANDLER(41)
DEFINE_HANDLER(42)
DEFINE_HANDLER(43)
DEFINE_HANDLER(44)
DEFINE_HANDLER(45)
DEFINE_HANDLER(46)
DEFINE_HANDLER(47)

INT_VECTOR_END


#endif // IHUB_H

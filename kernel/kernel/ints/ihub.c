#ifndef IHUB_H
#define IHUB_H

#include <kernel/arch/cpu/ints/ints-tool.h>

#include <libc/stdio.h>

void __exception(void)
{
    puts("BLYAT!\n");
}

I386_INT_DEF(0, __exception);
I386_INT_DEF(1, __exception);
I386_INT_DEF(2, __exception);
I386_INT_DEF(3, __exception);
I386_INT_DEF(4, __exception);
I386_INT_DEF(5, __exception);
I386_INT_DEF(6, __exception);
I386_INT_DEF(7, __exception);
I386_INT_DEF(8, __exception);
I386_INT_DEF(9, __exception);
I386_INT_DEF(10, __exception);
I386_INT_DEF(11, __exception);
I386_INT_DEF(12, __exception);
I386_INT_DEF(13, __exception);
I386_INT_DEF(14, __exception);
I386_INT_DEF(15, __exception);
I386_INT_DEF(16, __exception);
I386_INT_DEF(17, __exception);
I386_INT_DEF(18, __exception);
I386_INT_DEF(19, __exception);
I386_INT_DEF(20, __exception);
I386_INT_DEF(21, __exception);
I386_INT_DEF(22, __exception);
I386_INT_DEF(23, __exception);
I386_INT_DEF(24, __exception);
I386_INT_DEF(25, __exception);
I386_INT_DEF(26, __exception);
I386_INT_DEF(27, __exception);
I386_INT_DEF(28, __exception);
I386_INT_DEF(29, __exception);
I386_INT_DEF(30, __exception);
I386_INT_DEF(31, __exception);

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

INT_VECTOR_END


#endif // IHUB_H

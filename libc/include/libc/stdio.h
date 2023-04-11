#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include <libc/sys/cdefs.h>
#include <libc/stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* stdio.h header functions declaration. */

#define EOF (-1)

int putchar(int ch);
int puts(const char *str);
int printf(const char *restrict format, ... );

#ifdef __cplusplus
}
#endif

#endif // LIBC_STDIO_H

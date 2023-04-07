#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include <libc/sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* stdio.h header functions declaration. */

#define EOF (-1)
#define WRITE_SUCCESS (0)

int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif // LIBC_STDIO_H

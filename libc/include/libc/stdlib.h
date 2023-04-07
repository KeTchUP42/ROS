#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H

#include <libc/sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* stdlib.h header functions declaration. */

__attribute__((__noreturn__))
void abort(void);

__attribute__((__noreturn__))
void die(const char*);

#ifdef __cplusplus
}
#endif

#endif // LIBC_STDLIB_H

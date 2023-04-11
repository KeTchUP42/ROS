#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H

#include <libc/sys/cdefs.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Here is some basic general-purpose support macros:
*/
#define TO_STRING( X ) #X

#define __CONCAT_IMPL_2( A, B ) A##B
#define MACRO_CONCAT_2( A, B ) __CONCAT_IMPL_2( A, B )

#define EXIT_FAILURE (-1)
#define EXIT_SUCCESS (0)

/* Kernel init-fini system info: */

#ifdef KERNEL_CODE

#define SECTION( S ) __attribute__ ((section ( S )))
#define INIT_SECTION ".ctors"
#define FINI_SECTION ".dtors"

#endif

/* stdlib.h cmptor_type functions definition: */

typedef bool(*cmptor_type)(const void *, const void *);

static inline bool ptr_cmp(const void *lhs, const void *rhs)
{
    return (lhs == rhs);
}

/* stdlib.h header functions declaration: */

__attribute__((__noreturn__))
void abort(void);

__attribute__((__noreturn__))
void die(const char *str);

#ifdef __cplusplus
}
#endif

#endif // LIBC_STDLIB_H

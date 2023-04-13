#ifndef LIBC_TIME_H
#define LIBC_TIME_H

#include <libc/sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void sleep(size_t mls);

#ifdef __cplusplus
}
#endif

#endif // LIBC_TIME_H

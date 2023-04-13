#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include <libc/sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void *lhs, const void *rhs, size_t count);
void *memcpy(void *restrict dest, const void *restrict src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
void *memset(void *dest, int ch, size_t count);

size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif //LIBC_STRING_H

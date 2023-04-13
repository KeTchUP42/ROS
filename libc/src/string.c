#include <libc/string.h>

#include <stddef.h>
#include <stdint.h>

int memcmp(const void *lhs, const void *rhs, size_t count)
{
    const unsigned char *a = (const unsigned char *) lhs;
    const unsigned char *b = (const unsigned char *) rhs;
    for (size_t i = 0; i < count; ++i)
    {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}

void *memcpy(void *restrict dest, const void *restrict source, size_t count)
{
    unsigned char *dst = (unsigned char *) dest;
    const unsigned char *src = (const unsigned char *) source;
    for (size_t i = 0; i < count; ++i)
        dst[i] = src[i];
    return dest;
}

void *memmove(void *dest, const void *source, size_t count)
{
    unsigned char *dst = (unsigned char *) dest;
    const unsigned char *src = (const unsigned char *) source;
    if (dst < src)
    {
        for (size_t i = 0; i < count; ++i)
            dst[i] = src[i];
    }
    else
    {
        for (size_t i = count; i != 0; --i)
            dst[i - 1] = src[i - 1];
    }
    return dest;
}

void *memset(void *dest, int ch, size_t count)
{
    unsigned char *buff = (unsigned char *) dest;
    for (size_t i = 0; i < count; ++i)
        buff[i] = (unsigned char) ch;
    return dest;
}

/**
 * @brief Returns the length of the given null-terminated byte string,
 * that is, the number of characters in a character array whose first element
 * is pointed to by str up to and not including the first null character.
 *
 * The behavior is undefined if str is not a pointer to a null-terminated byte string.
 *
 * @param str Pointer to the null-terminated byte string to be examined.
 *
 * @return The length of the null-terminated byte string str.
 */
size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

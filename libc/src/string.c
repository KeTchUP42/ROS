#include <libc/string.h>

#include <stddef.h>

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

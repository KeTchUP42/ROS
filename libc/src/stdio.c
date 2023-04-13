#include <libc/stdio.h>

#include <libc/string.h>
#include <libc/stdlib.h>

#include <stdbool.h>
#include <stddef.h>

#include <stdarg.h>

#ifdef KERNEL_CODE
#include <kernel/tty/ktty.h>
#endif

int putchar(int ch)
{
#ifdef KERNEL_CODE
    tty_putchar(kernel_default_tty(), ch);
    return EXIT_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
    return ch;
}

int puts(const char *str)
{
#ifdef KERNEL_CODE
    tty_print(kernel_default_tty(), str);
    return EXIT_SUCCESS;
#else
    // TODO: write() syscall
    return EOF;
#endif
}

static void revc(char *lbuff, char *rbuff)
{
    char *p1 = lbuff;
    char *p2 = rbuff;
    char tmp;
    while (p1 < p2)
    {
        tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

static void uitoa(char *buff, char bs, unsigned int num)
{
    char *ptr = buff;
    int div = (bs == 'x') ? 16 : 10;
    unsigned int unum = num;

    int rmnd;
    do
    {
        rmnd = unum % div;
        *ptr++ = (rmnd < 10) ? (rmnd + '0') : (rmnd + 'a' - 10);
        unum /= div;
    }
    while (unum);

    /* Terminate BUFF. */
    *ptr = 0;

    /* Reverse BUFF. */
    revc(buff, (ptr - 1));
}

static void itoa(char *buff, char bs, int num)
{
    unsigned int unum;

    /* If %d is specified and NUM is negative, put ‘-’ in the head. */
    if ((bs == 'd') && (num < 0))
    {
        *buff = '-';
        buff++;
        unum = -num;
    }
    else
    {
        unum = (unsigned int)num;
    }

    uitoa(buff, bs, unum);
}

/* It's UB if there is no va_end() call before return statement in such function. */
#define FAIL_RET( VA_LIST )  \
    do { va_end(VA_LIST);    \
        return EXIT_FAILURE; } while(0)

int printf(const char *restrict format, ...)
{
    va_list params;
    va_start(params, format);

    char ch;
    int written = 0;

    /**
     * MAX_INT is only 10 chars(+1 for minus and +1 for NULL terminator) length.
     * And you need additional 9 chars for correct address arithmetic with MAX padding which value is 9.
     * Then we round the size up according to the alignment of the stack.
     */
    char buff[32];

    const char *str;
    const char *str_tmp;
    bool pad0;
    int pad;

    while ((ch = *format++) != 0)
    {
        if (ch != '%')
        {
            if (putchar(ch) == EOF)
                FAIL_RET(params);
            written++;
        }
        else
        {
            if (!(ch = *format++))
                FAIL_RET(params);

            if (ch == '%')
            {
                if (putchar(ch) == EOF)
                    FAIL_RET(params);
                written++;
                continue;
            }

            pad0 = false;
            pad = 0;

            if (ch == '0')
            {
                pad0 = true;
                if (!(ch = *format++))
                    FAIL_RET(params);
            }

            if ((ch >= '0') && (ch <= '9'))
            {
                pad = ch - '0';
                if (!(ch = *format++))
                    FAIL_RET(params);
            }

            switch (ch)
            {
                case 'd':
                case 'x':
                    itoa(buff, ch, va_arg(params, int));
                    str = buff;
                    goto str_out;
                    break;

                case 'u':
                    uitoa(buff, ch, va_arg(params, unsigned int));
                    str = buff;
                    goto str_out;
                    break;

                case 's':
                    str = va_arg(params, const char *);
                    if (str == NULL)
                        str = "(null)";

str_out:
                    for (str_tmp = str; *str_tmp; ++str_tmp);
                    for (; str_tmp < str + pad; ++str_tmp)
                        if (putchar(pad0 ? '0' : ' ') == EOF)
                            FAIL_RET(params);
                    while (*str)
                    {
                        if (putchar(*str++) == EOF)
                            FAIL_RET(params);
                        ++written;
                    }
                    break;

                case 'c':
                    if (putchar((char)va_arg(params, int)) == EOF)
                        FAIL_RET(params);
                    ++written;
                    break;

                /* For unknown formats: */
                default:
                    FAIL_RET(params);
            }
        }
    }
    va_end(params);
    return written;
}

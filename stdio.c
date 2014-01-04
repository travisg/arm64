#include <stdio.h>
#include <printf.h>
#include <debug.h>

static int _debug_output(const char *str, size_t len, void *state)
{
    size_t count = 0;
    while (count < len && *str) {
        debug_putchar(*str);
        str++;
        count++;
    }

    return count;
}


int _printf(const char *fmt, ...)
{
    int err;

    va_list ap;
    va_start(ap, fmt);

    err = _printf_engine(&_debug_output, NULL, fmt, ap);
    va_end(ap);

    return err;
}

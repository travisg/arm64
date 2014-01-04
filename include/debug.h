#pragma once

#include <stdint.h>

#define SEMIHOST_WRITEC (0x03)
#define SEMIHOST_WRITE0 (0x04)

uint32_t semihost_call(uint64_t op, void *ptr);

static inline void debug_puts(const char *ptr)
{
    semihost_call(SEMIHOST_WRITE0, (void *)ptr);
}

static inline void debug_putchar(int c)
{
    semihost_call(SEMIHOST_WRITEC, (void *)&c);
}


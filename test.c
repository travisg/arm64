#include "compiler.h"
#include "reg.h"

int a_global = 0;
int b_global = 1;

#define SYSREG_BASE (0x1c010000)
#define SYSREG_SWITCH  (SYSREG_BASE + 0x04)
#define SYSREG_LED  (SYSREG_BASE + 0x08)

#define NULL (0)

#define SEMIHOST_WRITE0 (0x04)

typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

uint32_t semihost_call(uint64_t op, void *ptr);

void main(void)
{
    semihost_call(SEMIHOST_WRITE0, "semihosted output!\n");
    *REG32(SYSREG_LED) = 0x55;

    for (;;)
        __asm__ volatile("wfi");
}

int foo(int a, int b)
{
    return a + b;
}

long foo64(long a, long b)
{
    return a + b;
}

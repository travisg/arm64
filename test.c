#include <stdint.h>
#include <sys/types.h>
#include <compiler.h>
#include <debug.h>
#include <reg.h>
#include <foundation_emu.h>
#include <stdio.h>
#include <arm64.h>

int a_global = 0;
int b_global = 1;

extern void arm64_exception_base(void);

void main(void)
{
    printf("welcome to arm64!\n");
    printf("a print from printf\n");
    printf("%d %ld\n", -1, -1L);
    set_led(0x55);

    unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;

    printf("el 0x%x\n", current_el);

    ARM64_WRITE_SYSREG(VBAR_EL1, (uint64_t)&arm64_exception_base);
    ARM64_WRITE_SYSREG(VBAR_EL2, (uint64_t)&arm64_exception_base);
    ARM64_WRITE_SYSREG(VBAR_EL3, (uint64_t)&arm64_exception_base);

    printf("before svc\n");
    __asm__ volatile("svc #99");
    printf("after svc\n");

    printf("%d\n", longargs(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

    for (;;)
        __asm__ volatile("wfi");
}

void sync_exception(void)
{
    printf("sync_exception\n");

    for (;;)
        ;
}

int foo(int a, int b)
{
    return a + b;
}

long foo64(long a, long b)
{
    return a + b;
}

__NO_INLINE int longargs(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    return a + b + c + d + e + f + g + h + i;
}

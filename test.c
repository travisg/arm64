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
    printf("before svc2\n");
    __asm__ volatile("svc #100");
    printf("after svc2\n");

    printf("%d\n", longargs(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

    for (;;)
        __asm__ volatile("wfi");
}

static void dump_iframe(const struct arm64_iframe_long *iframe)
{
    printf("iframe %p:\n");
    printf("x0  0x%16lx x1  0x%16lx x2  0x%16lx x3  0x%16lx\n", iframe->r[0], iframe->r[1], iframe->r[2], iframe->r[3]);
    printf("x4  0x%16lx x5  0x%16lx x6  0x%16lx x7  0x%16lx\n", iframe->r[4], iframe->r[5], iframe->r[6], iframe->r[7]);
    printf("x8  0x%16lx x9  0x%16lx x10 0x%16lx x11 0x%16lx\n", iframe->r[8], iframe->r[9], iframe->r[10], iframe->r[11]);
    printf("x12 0x%16lx x13 0x%16lx x14 0x%16lx x15 0x%16lx\n", iframe->r[12], iframe->r[13], iframe->r[14], iframe->r[15]);
    printf("x16 0x%16lx x17 0x%16lx x18 0x%16lx x19 0x%16lx\n", iframe->r[16], iframe->r[17], iframe->r[18], iframe->r[19]);
    printf("x20 0x%16lx x21 0x%16lx x22 0x%16lx x23 0x%16lx\n", iframe->r[20], iframe->r[21], iframe->r[22], iframe->r[23]);
    printf("x24 0x%16lx x25 0x%16lx x26 0x%16lx x27 0x%16lx\n", iframe->r[24], iframe->r[25], iframe->r[26], iframe->r[27]);
    printf("x28 0x%16lx x29 0x%16lx lr  0x%16lx sp  0x%16lx\n", iframe->r[28], iframe->r[29], iframe->r[30], iframe->r[31]);
    printf("elr 0x%16lx\n", iframe->elr);
    printf("spsr 0x%16lx\n", iframe->spsr);
}

void sync_exception(struct arm64_iframe_long *iframe)
{
    printf("sync_exception\n");
    dump_iframe(iframe);
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

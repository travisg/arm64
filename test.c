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

void main(void)
{
    printf("welcome to arm64!\n");
    printf("a print from printf\n");
    printf("%d %ld\n", -1, -1L);
    set_led(0x55);

    printf("el 0x%x\n", arm64_read_current_el());

    uint64_t foo = arm64_read_VBAR_EL1();
    printf("vtor 0x%llx\n", foo);

    arm64_write_VBAR_EL1(0x80000000);
    foo = arm64_read_VBAR_EL1();
    printf("vtor 0x%llx\n", foo);

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

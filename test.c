#include <stdint.h>
#include <sys/types.h>
#include <compiler.h>
#include <debug.h>
#include <reg.h>
#include <foundation_emu.h>
#include <stdio.h>

int a_global = 0;
int b_global = 1;

void main(void)
{
    printf("welcome to arm64!\n");
    printf("a print from printf\n");
    printf("%d %ld\n", -1, -1L);
    set_led(0x55);

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

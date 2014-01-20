#include <arm64.h>
#include <stdio.h>
#include <debug.h>
#include <foundation_emu.h> // for shutdown()

#define SHUTDOWN_ON_FATAL 1

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

static void num(uint64_t a)
{
    for (int i = 0; i < 16; i++) {
        debug_putchar("0123456789abcdef"[a & 0xf]);
        a>>=4;
    }
    debug_putchar('\n');
}

void arm64_sync_exception(struct arm64_iframe_long *iframe)
{
#if 0
    debug_puts("sync\n");
    num(iframe->elr);
    num(iframe->r[31]);
    num(ARM64_READ_SYSREG(esr_el1));
    num(ARM64_READ_SYSREG(currentel));
    for (;;);
#endif
    printf("sync_exception\n");
    dump_iframe(iframe);

    uint32_t esr = ARM64_READ_SYSREG(esr_el1);
    uint32_t ec = esr >> 26;
    uint32_t il = (esr >> 25) & 0x1;
    uint32_t iss = esr & ((1<<24) - 1);

    printf("ESR 0x%x: ec 0x%x, il 0x%x, iss 0x%x\n", esr, ec, il, iss);

    if (ec == 0x15) { // syscall
        printf("syscall\n");
        return;
    }

#if SHUTDOWN_ON_FATAL
    shutdown();
#else
    for (;;);
#endif
}

void arm64_invalid_exception(struct arm64_iframe_long *iframe, unsigned int which)
{
    printf("invalid exception, which 0x%u\n", which);
    dump_iframe(iframe);

#if SHUTDOWN_ON_FATAL
    shutdown();
#else
    for (;;);
#endif
}



#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <debug.h>
#include <reg.h>
#include <foundation_emu.h>
#include <arm64.h>

/* driver for GICv2 as implemented in Foundation v2 */

static inline void enter_critical_section(void) {}
static inline void exit_critical_section(void) {}

#define NO_ERROR 0

#define GICCPUREG(reg) (*REG32(GIC_PROC_BASE + (reg)))
#define GICDISTREG(reg) (*REG32(GIC_DISTRIB_BASE + (reg)))

/* main cpu regs */
#define CONTROL  (0x00)
#define PMR      (0x04)
#define BR       (0x08)
#define IAR      (0x0c)
#define EOIR     (0x10)
#define RPR      (0x14)
#define HPPIR    (0x18)
#define ABPR     (0x1c)
#define AIAR     (0x20)
#define AEOIR    (0x24)
#define AHPPIR   (0x28)

/* distribution regs */
#define DISTCONTROL (0x000)
#define GROUP       (0x080)
#define SETENABLE   (0x100)
#define CLRENABLE   (0x180)
#define SETPEND     (0x200)
#define CLRPEND     (0x280)
#define SETACTIVE   (0x300)
#define CLRACTIVE   (0x380)
#define PRIORITY    (0x400)
#define TARGET      (0x800)
#define CONFIG      (0xc00)
#define NSACR       (0xe00)
#define SGIR        (0xf00)

static void gic_set_enable(uint vector, bool enable)
{
    if (enable) {
        uint regoff = SETENABLE + 4 * (vector / 32);
        GICDISTREG(regoff) = (1 << (vector % 32));
    } else {
        uint regoff = CLRENABLE + 4 * (vector / 32);
        GICDISTREG(regoff) = (1 << (vector % 32));
    }
}

void interrupt_init(void)
{
    GICDISTREG(DISTCONTROL) = 0;

    GICDISTREG(CLRENABLE) = 0xffff0000;
    GICDISTREG(SETENABLE) = 0x0000ffff;
    GICDISTREG(CLRPEND) = 0xffffffff;
    GICDISTREG(GROUP) = 0;
    GICCPUREG(PMR) = 0xf0;

    for (int i = 0; i < 32 / 4; i++) {
        GICDISTREG(PRIORITY + i * 4) = 0x80808080;
    }

    for (int i = 32/16; i < MAX_INT / 16; i++) {
        GICDISTREG(NSACR + i * 4) = 0xffffffff;
    }
    for (int i = 32/32; i < MAX_INT / 32; i++) {
        GICDISTREG(CLRENABLE + i * 4) = 0xffffffff;
        GICDISTREG(CLRPEND + i * 4) = 0xffffffff;
        GICDISTREG(GROUP + i * 4) = 0xffffffff;
    }

    for (int i = 32/4; i < MAX_INT / 4; i++) {
        GICDISTREG(TARGET + i * 4) = 0;
        GICDISTREG(PRIORITY + i * 4) = 0x80808080;
    }

    GICDISTREG(DISTCONTROL) = 3; // enable GIC0
    GICCPUREG(CONTROL) = (1<<3)|3; // enable GIC0

#if 0
    hexdump((void *)GIC_PROC_BASE, 0x20);
    hexdump((void *)GIC_DISTRIB_BASE, 0x10);
    printf("config:   "); hexdump((void *)GIC_DISTRIB_BASE + CONFIG, 0x10);
    printf("group:    "); hexdump((void *)GIC_DISTRIB_BASE + GROUP, 0x10);
    printf("priority: "); hexdump((void *)GIC_DISTRIB_BASE + PRIORITY, 0x40);
    printf("enable:   "); hexdump((void *)GIC_DISTRIB_BASE + SETENABLE, 0x10);
    printf("pending:  "); hexdump((void *)GIC_DISTRIB_BASE + SETPEND, 0x10);
    printf("active:   "); hexdump((void *)GIC_DISTRIB_BASE + SETACTIVE, 0x10);

    // trigger interrupt
    gic_set_enable(34, true);
    //GICDISTREG(SETPEND + 4) = (1<<2);
    //GICDISTREG(SETACTIVE + 4) = (1<<2);
    GICDISTREG(SGIR) = (2 << 24) | 3;

    printf("ISR 0x%x\n", (uint32_t)ARM64_READ_SYSREG(isr_el1));

    printf("daif 0x%x\n", (uint32_t)ARM64_READ_SYSREG(daif));
    arch_enable_interrupts();

    printf("daif 0x%x\n", (uint32_t)ARM64_READ_SYSREG(daif));
#endif
}

status_t mask_interrupt(unsigned int vector)
{
    if (vector >= MAX_INT)
        return -1;

    enter_critical_section();

    gic_set_enable(vector, false);

    exit_critical_section();

    return NO_ERROR;
}

status_t unmask_interrupt(unsigned int vector)
{
    if (vector >= MAX_INT)
        return -1;

    enter_critical_section();

    gic_set_enable(vector, true);

    exit_critical_section();

    return NO_ERROR;
}

void _irq(struct arm64_iframe_long *frame)
{
    uint32_t iar = GICCPUREG(AIAR);
    uint vector = iar & 0x3ff;

    if (vector >= 0x3fe) {
        // spurious
        return;
    }

//    printf("irq %d\n", vector);

    switch (vector) {
        case INT_TIMER0:
            timer_irq();
            break;
    }

    GICCPUREG(AEOIR) = iar;
}

void _fiq(struct arm64_iframe_long *frame)
{
    uint32_t iar = GICCPUREG(IAR);
    uint vector = iar & 0x3ff;

    if (vector >= 0x3fe) {
        // spurious
        return;
    }

//    printf("fiq %d\n", vector);

    GICCPUREG(EOIR) = iar;
}


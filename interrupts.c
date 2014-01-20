#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <debug.h>
#include <reg.h>
#include <foundation_emu.h>
#include <arm64.h>

static inline void enter_critical_section(void) {}
static inline void exit_critical_section(void) {}

#define NO_ERROR 0

#define GICCPUREG(reg) (*REG32(GIC_PROC_BASE + (reg)))
#define GICDISTREG(reg) (*REG32(GIC_DISTRIB_BASE + (reg)))

/* main cpu regs */
#define CONTROL  (0x00)
#define PRIMASK  (0x04)
#define BINPOINT (0x08)
#define INTACK   (0x0c)
#define EOI      (0x10)
#define RUNNING  (0x14)
#define HIGHPEND (0x18)

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
#define SGIR        (0xf00)

static void gic_set_enable(uint vector, bool enable)
{
    if (enable) {
        uint regoff = SETENABLE + 4 * (vector / 32); // (vector < 32) ? SETENABLE0 : ((vector < 64) ? SETENABLE1 : SETENABLE2);
        GICDISTREG(regoff) = (1 << (vector % 32));
    } else {
        uint regoff = CLRENABLE + 4 * (vector / 32); // (vector < 32) ? CLRENABLE0 : ((vector < 64) ? CLRENABLE1 : CLRENABLE2);
        GICDISTREG(regoff) = (1 << (vector % 32));
    }
}

void platform_init_interrupts(void)
{
    GICDISTREG(DISTCONTROL) = 0;

    GICDISTREG(CLRENABLE) = 0xffff0000;
    GICDISTREG(SETENABLE) = 0x0000ffff;
    GICDISTREG(CLRPEND) = 0xffffffff;
    GICDISTREG(GROUP) = 0x0;
    GICDISTREG(PRIMASK) = 0xf0;

    for (int i = 0; i < 32 / 4; i++) {
        GICDISTREG(PRIORITY + i * 4) = 0xa0a0a0a0;
    }

    for (int i = 32/32; i < MAX_INT / 32; i++) {
        GICDISTREG(CLRENABLE + i * 4) = 0xffffffff;
        GICDISTREG(CLRPEND + 4) = 0xffffffff;
        GICDISTREG(GROUP + 4) = 0xffffffff;
    }

    for (int i = 32/16; i < MAX_INT / 16; i++) {
        GICDISTREG(CONFIG + i * 4) = 0;
    }
    for (int i = 32/4; i < MAX_INT / 4; i++) {
        GICDISTREG(TARGET + i * 4) = 0;
        GICDISTREG(PRIORITY + i * 4) = 0xa0a0a0a0;
    }

    GICCPUREG(CONTROL) = 1; // enable GIC0
    GICDISTREG(DISTCONTROL) = 1; // enable GIC0

    hexdump((void *)GIC_DISTRIB_BASE + 0xfd0, 0x30);
    hexdump((void *)GIC_PROC_BASE, 0x20);
    hexdump((void *)GIC_PROC_BASE + 0xf0, 4);
    hexdump((void *)GIC_DISTRIB_BASE, 0x10);
    hexdump((void *)GIC_DISTRIB_BASE + GROUP, 0x10);
    hexdump((void *)GIC_DISTRIB_BASE + SETENABLE, 0x10);
    hexdump((void *)GIC_DISTRIB_BASE + SETACTIVE, 0x10);
    hexdump((void *)GIC_DISTRIB_BASE + PRIORITY, 0x40);

    // trigger interrupt
    gic_set_enable(34, true);
    GICDISTREG(SETPEND + 4) = (1<<2);
    //GICDISTREG(SETACTIVE + 4) = (1<<2);
    GICDISTREG(SGIR) = (2 << 24) | 3;

    hexdump((void *)GIC_DISTRIB_BASE + SETENABLE, 0x10);
    hexdump((void *)GIC_DISTRIB_BASE + SETPEND, 0x10);
    hexdump((void *)GIC_DISTRIB_BASE + SETACTIVE, 0x10);
    hexdump((void *)GIC_PROC_BASE, 0x20);

    printf("daif 0x%x\n", (uint32_t)ARM64_READ_SYSREG(daif));
    arch_enable_interrupts();

    printf("ISR 0x%x\n", (uint32_t)ARM64_READ_SYSREG(isr_el1));
    printf("daif 0x%x\n", (uint32_t)ARM64_READ_SYSREG(daif));
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

#if 0
enum handler_return platform_irq(struct arm_iframe *frame)
{
    // get the current vector
    unsigned int vector = GICREG(0, INTACK) & 0x3ff;

    // see if it's spurious
    if (vector == 0x3ff) {
        GICREG(0, EOI) = 0x3ff; // XXX is this necessary?
        return INT_NO_RESCHEDULE;
    }

//  printf("platform_irq: spsr 0x%x, pc 0x%x, currthread %p, vector %d\n", frame->spsr, frame->pc, current_thread, vector);

    // deliver the interrupt
    enum handler_return ret;

    ret = INT_NO_RESCHEDULE;
    if (int_handler_table[vector].handler)
        ret = int_handler_table[vector].handler(int_handler_table[vector].arg);

    GICREG(0, EOI) = vector;

//  printf("platform_irq: exit %d\n", ret);

    KEVLOG_IRQ_EXIT(vector);

    return ret;
}

void platform_fiq(struct arm_iframe *frame)
{
    PANIC_UNIMPLEMENTED;
}
#endif


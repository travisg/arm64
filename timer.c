#include <timer.h>
#include <reg.h>
#include <stdio.h>
#include <interrupts.h>
#include <foundation_emu.h>
#include <arm64.h>

#define DUMPREG(x) do { printf(#x " %#llx\n", ARM64_READ_SYSREG(x)); } while (0)

#if 0
/* implementation of SP804 dual-timer */

#define TIMREG(reg) (*REG32(TIMER0_BASE + (reg)))
#define TIMREG8(reg) (*REG8(TIMER0_BASE + (reg)))

#define LOADVAL (0x00)
#define VAL     (0x04)
#define CONTROL (0x08)
#define INTCLEAR (0x0c)
#define RAWINTSTAT (0x10)
#define MASKEDINTSTAT (0x14)

void timer_start(unsigned int interval)
{
    printf("starting timer\n");
    TIMREG(LOADVAL) = interval; /* XXX 50 seems about right for milliseconds */

    TIMREG(CONTROL) |= (1<<7); // enable

#if 0
    for (;;)
        printf("%d %d\n", TIMREG(VAL), TIMREG(RAWINTSTAT));
#endif
}

void timer_irq(void)
{
    //printf("timer tick\n");

    static int count = 0;
    count++;
    if ((count % 100) == 0)
        printf("%d\n", count);

    TIMREG(INTCLEAR) = 1;
}

void timer_init(void)
{
    /* disable timer */
    TIMREG(CONTROL) = 0;

    /* periodic mode, ints enabled, 32bit, wrapping */
    TIMREG(CONTROL) = (1<<6)|(1<<5)|(1<<1);

    unmask_interrupt(INT_TIMER0);
//    register_int_handler(TIMER01_INT, &platform_tick, NULL);
}

#else

/* armv8 specified timer */

/* relative to CNTControl */
#define CNTCR 0x00
#define CNTSR 0x04
#define CNTCV 0x08
#define CNTFID0 0x20

/* relative to CNTReadBase */
#define CNTCV2 0x00

static uint64_t interval_delta;
static uint64_t last_compare;
static uint64_t timer_freq;

static void set_compare_timer(uint64_t interval)
{
    last_compare = ARM64_READ_SYSREG(CNTPCT_EL0) + interval;
    ARM64_WRITE_SYSREG(CNTP_CVAL_EL0, last_compare);
}

void timer_start(unsigned int interval)
{
    /* disable the timer */
    ARM64_WRITE_SYSREG(CNTP_CTL_EL0, 0);

    interval_delta = timer_freq / interval;
    set_compare_timer(interval_delta);

    ARM64_WRITE_SYSREG(CNTP_CTL_EL0, 1);
}

void timer_irq(void)
{
    static int count = 0;

    /* reset the compare register ahead of the physical counter */
    set_compare_timer(interval_delta);

    count++;
    if ((count % 100) == 0)
        printf("%d\n", count);
}

void timer_init(void)
{
    /* read the base frequency from the control block */
    timer_freq = *REG32(REFCLK_CNTControl + CNTFID0);
    printf("timer running at %lld Hz\n", timer_freq);

    /* start the physical timer */
    *REG32(REFCLK_CNTControl + CNTCR) = 1;

    unmask_interrupt(INT_PPI_NSPHYS_TIMER);
}

#endif



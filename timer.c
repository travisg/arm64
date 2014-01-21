#include <timer.h>
#include <reg.h>
#include <stdio.h>
#include <interrupts.h>
#include <foundation_emu.h>

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
    TIMREG(LOADVAL) = interval * 50; /* XXX 50 seems about right for milliseconds */

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




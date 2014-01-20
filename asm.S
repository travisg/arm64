#include "asm.h"

FUNCTION(_start)
    ldr x0, =stack_end
    mov sp, x0

    bl  main
    b   .

FUNCTION(semihost_call)
    hlt 0xf000
    ret

.macro set_led, n
    ldr x20, =0x1c010000+0x8
    mov w21,#\n
    str w21, [x20]
.endm


FUNCTION(arm64_el3_to_el1)
    /* set EL2 to 64bit */
    mrs x0, scr_el3
    orr x0, x0, #(1<<10)
    msr scr_el3, x0

    /* set EL1 to 64bit */
    mov x0, #(1<<31)
    msr hcr_el2, x0

    /* disable EL2 coprocessor traps */
    mov x0, #0x33ff
    msr cptr_el2, x0

    /* disable EL1 FPU traps */
    mov x0, #(0b11<<20)
    msr cpacr_el1, x0

    /* set up the EL1 bounce interrupt */
    mov x0, sp
    msr sp_el1, x0

    adr x0, .Ltarget
    msr elr_el3, x0

    mov x0, #((0b1111 << 6) | (0b0101)) /* EL1h runlevel */
    msr spsr_el3, x0
    isb

    set_led 1
    eret

.Ltarget:
    set_led 2
    ret

.bss
    .align 4
DATA(stack)
    .skip 0x2000
DATA(stack_end)

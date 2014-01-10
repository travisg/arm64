#include "asm.h"

FUNCTION(_start)
    ldr x0, =stack_end
    mov sp, x0

    bl  main
    b   .

FUNCTION(semihost_call)
    hlt 0xf000
    ret

.bss
    .align 4
DATA(stack)
    .skip 0x2000
DATA(stack_end)

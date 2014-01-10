#include <asm.h>

.section .text.boot.vectab1

FUNCTION(ex1)
    ret

.org 0x200
FUNCTION(ex2)
    ret

.org 0x400
FUNCTION(ex3)
    ret

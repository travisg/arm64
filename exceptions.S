#include <asm.h>

.section .text.boot.vectab1

.macro push ra, rb
stp \ra, \rb, [sp,#-16]!
.endm

.macro pop ra, rb
ldp \ra, \rb, [sp], #16
.endm

#define lr x30
#define regsave_long_offset 0xf0

.macro regsave_long
sub  sp, sp, #32
push x28, x29
push x26, x27
push x24, x25
push x22, x23
push x20, x21
push x18, x19
push x16, x17
push x14, x15
push x12, x13
push x10, x11
push x8, x9
push x6, x7
push x4, x5
push x2, x3
push x0, x1
add  x0, sp, #regsave_long_offset
mrs  x1, elr_el3
mrs  x2, spsr_el3
stp  lr, x0, [sp, #regsave_long_offset]
stp  x1, x2, [sp, #regsave_long_offset + 16]
.endm

.macro regrestore_long
ldr  lr, [sp, #regsave_long_offset]
ldp  x1, x2, [sp, #regsave_long_offset + 16]
msr  elr_el3, x1
msr  spsr_el3, x2
pop x0, x1
pop x2, x3
pop x4, x5
pop x6, x7
pop x8, x9
pop x10, x11
pop x12, x13
pop x14, x15
pop x16, x17
pop x18, x19
pop x20, x21
pop x22, x23
pop x24, x25
pop x26, x27
pop x28, x29
add sp, sp, #32
.endm

FUNCTION(arm64_exception_base)

/* exceptions from current EL, using SP0 */
LOCAL_FUNCTION(arm64_sync_exc_current_el_SP0)
    eret

.org 0x080
LOCAL_FUNCTION(arm64_irq_current_el_SP0)
    eret

.org 0x100
LOCAL_FUNCTION(arm64_fiq_current_el_SP0)
    eret

.org 0x180
LOCAL_FUNCTION(arm64_err_exc_current_el_SP0)
    eret

/* exceptions from current EL, using SPx */
.org 0x200
LOCAL_FUNCTION(arm64_sync_exc_current_el_SPx)
    regsave_long
    adr lr, arm64_exc_shared_restore
    mov x0, sp
    b  sync_exception

.org 0x280
LOCAL_FUNCTION(arm64_irq_current_el_SPx)
    eret

.org 0x300
LOCAL_FUNCTION(arm64_fiq_current_el_SPx)
    eret

.org 0x380
LOCAL_FUNCTION(arm64_err_exc_current_el_SPx)
    eret

/* exceptions from lower EL, running arm64 */
.org 0x400
LOCAL_FUNCTION(arm64_sync_exc_lower_el_64)
    eret

.org 0x480
LOCAL_FUNCTION(arm64_irq_lower_el_64)
    eret

.org 0x500
LOCAL_FUNCTION(arm64_fiq_lower_el_64)
    eret

.org 0x580
LOCAL_FUNCTION(arm64_err_exc_lower_el_64)
    eret

/* exceptions from lower EL, running arm32 */
.org 0x600
LOCAL_FUNCTION(arm64_sync_exc_lower_el_32)
    eret

.org 0x680
LOCAL_FUNCTION(arm64_irq_lower_el_32)
    eret

.org 0x700
LOCAL_FUNCTION(arm64_fiq_lower_el_32)
    eret

.org 0x780
LOCAL_FUNCTION(arm64_err_exc_lower_el_32)
    eret

LOCAL_FUNCTION(arm64_exc_shared_restore)
    regrestore_long
    eret

#include <asm.h>

.section .text.boot.vectab1

FUNCTION(arm64_exception_base)

/* exceptions from current EL, using SP0 */
FUNCTION(arm64_sync_exc_current_el_SP0)
    eret

.org 0x080
FUNCTION(arm64_irq_current_el_SP0)
    eret

.org 0x100
FUNCTION(arm64_fiq_current_el_SP0)
    eret

.org 0x180
FUNCTION(arm64_err_exc_current_el_SP0)
    eret

/* exceptions from current EL, using SPx */
.org 0x200
FUNCTION(arm64_sync_exc_current_el_SPx)
    bl  sync_exception
    eret

.org 0x280
FUNCTION(arm64_irq_current_el_SPx)
    eret

.org 0x300
FUNCTION(arm64_fiq_current_el_SPx)
    eret

.org 0x380
FUNCTION(arm64_err_exc_current_el_SPx)
    eret

/* exceptions from lower EL, running arm64 */
.org 0x400
FUNCTION(arm64_sync_exc_lower_el_64)
    eret

.org 0x480
FUNCTION(arm64_irq_lower_el_64)
    eret

.org 0x500
FUNCTION(arm64_fiq_lower_el_64)
    eret

.org 0x580
FUNCTION(arm64_err_exc_lower_el_64)
    eret

/* exceptions from lower EL, running arm32 */
.org 0x600
FUNCTION(arm64_sync_exc_lower_el_32)
    eret

.org 0x680
FUNCTION(arm64_irq_lower_el_32)
    eret

.org 0x700
FUNCTION(arm64_fiq_lower_el_32)
    eret

.org 0x780
FUNCTION(arm64_err_exc_lower_el_32)
    eret

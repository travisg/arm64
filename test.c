#include <stdint.h>
#include <sys/types.h>
#include <compiler.h>
#include <debug.h>
#include <reg.h>
#include <foundation_emu.h>
#include <stdio.h>
#include <arm64.h>

void main(void)
{
    printf("welcome to arm64!\n");
    printf("a print from printf\n");
    printf("%d %ld\n", -1, -1L);
    //set_led(0x55);

    ARM64_WRITE_SYSREG(VBAR_EL1, (uint64_t)&arm64_exception_base);
    ARM64_WRITE_SYSREG(VBAR_EL2, (uint64_t)&arm64_exception_base);
    ARM64_WRITE_SYSREG(VBAR_EL3, (uint64_t)&arm64_exception_base);

    printf("SCR_EL3 0x%x\n", ARM64_READ_SYSREG(SCR_EL3));
    printf("SCTLR_EL1 0x%x\n", ARM64_READ_SYSREG(SCTLR_EL1));
    printf("SCTLR_EL2 0x%x\n", ARM64_READ_SYSREG(SCTLR_EL2));
    printf("SCTLR_EL3 0x%x\n", ARM64_READ_SYSREG(SCTLR_EL3));
    printf("HCR_EL2 0x%x\n", ARM64_READ_SYSREG(HCR_EL2));
    printf("HSTR_EL2 0x%x\n", ARM64_READ_SYSREG(HSTR_EL2));
    printf("CPTR_EL3 0x%x\n", ARM64_READ_SYSREG(CPTR_EL3));
    printf("CPTR_EL2 0x%x\n", ARM64_READ_SYSREG(CPTR_EL2));
    printf("CPACR_EL1 0x%x\n", ARM64_READ_SYSREG(CPACR_EL1));

#if 1
    unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;
    printf("el 0x%x\n", current_el);
    printf("switching to el1\n");
    arm64_el3_to_el1();
    current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;
    printf("el 0x%x\n", current_el);

    printf("before svc\n");
    __asm__ volatile("svc #99");
    printf("after svc\n");
    printf("before svc2\n");
    __asm__ volatile("svc #100");
    printf("after svc2\n");
#endif

    platform_init_interrupts();

    printf("stopping simulation\n");
    shutdown();
}

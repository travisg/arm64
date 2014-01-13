#pragma once

#include <stdint.h>

#define DSB __asm__ volatile("dsb" ::: "memory")
#define ISB __asm__ volatile("isb" ::: "memory")

#define ARM64_READ_SYSREG(reg) \
({ \
    uint64_t _val; \
    __asm__ volatile("mrs %0," #reg : "=r" (_val)); \
    _val; \
})

#define ARM64_WRITE_SYSREG(reg, val) \
({ \
    __asm__ volatile("msr " #reg ", %0" :: "r" (val)); \
    ISB; \
})

/* exception handling */
struct arm64_iframe_long {
    uint64_t r[32];
    uint64_t elr;
    uint64_t spsr;
};


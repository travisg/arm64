#pragma once

#include <stdint.h>

#define ISB

#define GEN_SYS_REG_FUNCS(reg) \
static inline __ALWAYS_INLINE uint64_t arm64_read_##reg(void) { \
    uint64_t val; \
    __asm__ volatile("mrs %0," #reg : "=r" (val)); \
    return val; \
} \
\
static inline __ALWAYS_INLINE void arm64_write_##reg(uint64_t val) { \
    __asm__ volatile("msr " #reg ", %0" : "=r" (val)); \
    ISB; \
}

GEN_SYS_REG_FUNCS(VBAR_EL1)
GEN_SYS_REG_FUNCS(VBAR)

static inline __ALWAYS_INLINE uint32_t arm64_read_current_el(void)
{
    uint32_t val;
    __asm__ volatile("mrs %0,currentel" : "=r" (val));
    return val;
}



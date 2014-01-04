#pragma once

#include <stdint.h>

#define SYSREG_BASE     (0x1c010000)
#define SYSREG_SWITCH   (SYSREG_BASE + 0x04)
#define SYSREG_LED      (SYSREG_BASE + 0x08)

static inline void set_led(uint8_t led)
{
    *REG32(SYSREG_LED) = led;
}

static inline uint8_t read_switches(void)
{
    return *REG32(SYSREG_SWITCH) & 0xff;
}


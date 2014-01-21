#pragma once

#include <sys/types.h>

void interrupt_init(void);
status_t mask_interrupt(unsigned int vector);
status_t unmask_interrupt(unsigned int vector);


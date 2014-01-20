#include <debug.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>

void hexdump(const void *ptr, size_t len)
{
    addr_t address = (addr_t)ptr;
    size_t count;
    int i;

    for (count = 0 ; count < len; count += 16) {
        printf("0x%08lx: ", address);
        printf("%08x %08x %08x %08x |", *(const uint32_t *)address, *(const uint32_t *)(address + 4), *(const uint32_t *)(address + 8), *(const uint32_t *)(address + 12));
        for (i=0; i < 16; i++) {
            char c = *(const char *)(address + i);
            if (c >= 0x20 && c < 0x7f) {
                printf("%c", c);
            } else {
                printf(".");
            }
        }
        printf("|\n");
        address += 16;
    }
}

void hexdump8(const void *ptr, size_t len)
{
    addr_t address = (addr_t)ptr;
    size_t count;
    int i;

    for (count = 0 ; count < len; count += 16) {
        printf("0x%08lx: ", address);
        for (i=0; i < 16; i++) {
            printf("0x%02hhx ", *(const uint8_t *)(address + i));
        }
        printf("\n");
        address += 16;
    }
}



#include <stdint.h>
#include <stddef.h>

#include <stdc/string.h>

int set_pitdiv(int freq) {
    uint32_t div = 1193180 / freq;
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(div & 0xFF);
    uint8_t h = (uint8_t)( (div >> 8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}

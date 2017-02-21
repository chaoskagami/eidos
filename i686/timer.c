#include <stdint.h>
#include <stddef.h>

#include <idt.h>
#include <utils.h>

#include <stdc/string.h>

void set_pitdiv(int freq) {
    uint32_t div = 1193180 / freq;
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(div & 0xFF);
    uint8_t h = (uint8_t)( (div >> 8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}

uint64_t ticks = 0;

uint64_t clock() {
    return ticks;
}

void clock_inc(__attribute__((unused)) struct interrupt_frame* frame) {
    ++ticks;
}

void i686_pit_init() {
    register_int_handler(32, clock_inc);
    set_pitdiv(CONF_TIMER_FREQ);
}

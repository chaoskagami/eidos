#include <stdint.h>
#include <module.h>
#include <module_console.h>

#define PORT 0x3f8   /* COM1 */

int is_transmit_empty() {
    return inb(PORT + 5) & 0x20;
}

void serial_put(char c, char color_fg, char color_bg, uint16_t x, uint16_t y) {
    while (is_transmit_empty() == 0);
    outb(PORT, c);
}

void serial_get(char* c, char* color_fg, char* color_bg, uint16_t x, uint16_t y) {
}

int serial_tty() {
    return 0;
}

struct console_module serial_module = {
    .put     = serial_put,
    .get     = serial_get,
    .tty     = serial_tty
};

void serial_init() {
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);    //                  (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set

    console_register_module(&serial_module);
}

void serial_deinit() {
    console_unregister_module(&serial_module);
    // Stub.
}

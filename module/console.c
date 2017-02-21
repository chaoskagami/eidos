/* This is a generic-ish interface for console drivers. The main reason for this is to abstract printing from what it is
actually printing to; in the future, adding a new backend for framebuffer rendering will be easy, for instance. */

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <module.h>
#include <module_console.h>

struct output *active;

void put_handler(char c) {
    active->out(c);
}

void console_reg(struct output* out) {
    active = out;
}

void console_unreg(struct output* out) {
    active = out;
}

void console_init() {
    // Temporary
    biosvga_init();
}

void console_deinit() {
    // Temporary
    biosvga_deinit();
}

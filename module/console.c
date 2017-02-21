/* This is a generic-ish interface for console drivers. The main reason for this is to abstract printing from what it is
actually printing to; in the future, adding a new backend for framebuffer rendering will be easy, for instance. */

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <module.h>
#include <module_console.h>

struct output *out_active;
struct input *in_active;

void put_handler(char c) {
    out_active->out(c);
}

void console_reg_write(struct output* out) {
    out_active = out;
}

void console_unreg_write(struct output* out) {
    if (out_active == out)
        out_active = NULL;
}

void console_reg_read(struct input* out) {
    in_active = out;
}

void console_unreg_read(struct input* out) {
    if (in_active == out)
        in_active = NULL;
}

void console_init() {
    // Temporary
    biosvga_init();
}

void console_deinit() {
    // Temporary
    biosvga_deinit();
}

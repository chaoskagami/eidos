/* This is a generic-ish interface for console drivers. The main reason for this is to abstract printing from what it is
actually printing to; in the future, adding a new backend for framebuffer rendering will be easy, for instance. */

#include <stdint.h>
#include <stddef.h>
#include <module.h>
#include <module_console.h>

struct console_module *active;
uint16_t x, y, tab_w;

void biosvga_init();
void biosvga_deinit();

void console_register_module(struct console_module* module) {
    active = module;
}

void console_unregister_module(struct console_module* module) {
    if (active == module)
        active = NULL;
}

// TODO - Should this be part of the backend? Scrolling is slow.
void scroll() {
    if (y >= active->height()) {
        for(int cy = 1; cy <= active->height(); cy++) {
            for(int cx = 0; cx <= active->width(); cx++) {
                char c = 0;
                uint8_t fg = 0, bg = 0;
                active->get(&c, &fg, &bg, cx, cy);
                active->put(c,  0xf,  0,  cx, cy-1);
            }
        }
        --y;
        for(int cx = 0; cx < active->width(); cx++) {
            active->put(' ',  0xf,  0,  cx, y);
        }
    }
}

// TODO - Should this be part of the backend? Scrolling is slow.
void clear() {
    x = y = 0;
    for(int cy = 0; cy < active->height(); cy++) {
        for(int cx = 0; cx < active->width(); cx++) {
            active->put(' ',  0xf,  0x0,  cx, cy);
        }
    }
}

void kputc(char c) {
    switch(c) {
        case '\t':
            x += tab_w - (x % tab_w);
            break;
        case '\b':
            --x;
            break;
        case '\n':
            ++y;
        case '\r':
            x = 0;
            break;
        case ' ' ... '~':
            active->put(c, 0xf, 0x0, x, y);
        default:
            x += 1;
            break;
    }

    if (x > active->width()) {
        x = 0;
        ++y;
    }

    scroll();
    active->cursor(x, y);
}

void kwrite(char* str) {
    while(*str != 0) {
        kputc(*str);
        ++str;
    }
}

void console_init() {
    x = y = 0;
    tab_w = 4;

    // Temporary
    biosvga_init();

    clear();
}

void console_deinit() {
    // Temporary
    biosvga_deinit();
}


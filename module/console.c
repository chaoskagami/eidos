/* This is a generic-ish interface for console drivers. The main reason for this is to abstract printing from what it is
actually printing to; in the future, adding a new backend for framebuffer rendering will be easy, for instance. */

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <module.h>
#include <module_console.h>

struct console_module *active;
struct console_module *reg[2];
uint16_t x, y, tab_w;

void biosvga_init();
void biosvga_deinit();

void console_mark_active(int idx) {
    active = reg[idx];
}

void console_register_module(struct console_module* module) {
    reg[1] = reg[0];
    reg[0] = module;
}

void console_unregister_module(struct console_module* module) {
    if (reg[0] == module)
        reg[0] = reg[1];
    if (reg[1] == module)
        reg[1] = NULL;
}

// TODO - Should this be part of the backend? Scrolling is slow.
void scroll() {
    if (active->tty() == 0)
        return;

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
    if (active->tty() == 0)
        return;

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
    if (active->tty() == 1)
        active->cursor(x, y);
}

void kwrite(char* str) {
    while(*str != 0) {
        kputc(*str);
        ++str;
    }
}

int strcatlen(char* a, char* b) {
    char* o = b;
    while(*b != 0) {
        *a = *b;
        a++;
        b++;
    }
    return b - o;
}

int dumphex(char* buf, const char* index, unsigned int num) {
    uint8_t *num_8 = (uint8_t *)&num;
    char *obuf = buf;
    for (int i = 3; i >= 0; i--) {
        uint8_t high = (num_8[i] >> 4) & 0xf;
        uint8_t low = num_8[i] & 0xf;

        *buf = (index)[high]; buf++;
        *buf = (index)[low];  buf++;
    }

    return buf - obuf;
}

void kvsprintf(char *buffer, const char* fmt_con, va_list ap) {
    char* fmt = (char*)fmt_con;
    char* buf = buffer;
    while(*fmt != 0) {
        if (*fmt == '%') {
            // Format string.
            ++fmt;
            switch (*fmt) {
                case 'c':
                    *buf = (char)va_arg(ap, int);
                    buf++;
                    fmt++;
                    break;
                case '%':
                    *buf = '%';
                    buf++;
                    fmt++;
                    break;
                case 's':
                    buf += strcatlen(buf, va_arg(ap, char*));
                    fmt++;
                    break;
                case 'x':
                    buf += dumphex(buf, "0123456789abcdef", va_arg(ap, unsigned int));
                    ++fmt;
                    break;
                case 'X':
                    buf += dumphex(buf, "0123456789ABCDEF", va_arg(ap, unsigned int));
                    ++fmt;
                    break;
                case 'd':
                case 'u':
                default:
                    ++fmt;
                    break;
            }
        } else {
            *buf = *fmt;
            ++buf;
            ++fmt;
        }
    }

    *buf = 0;
}

char buf[4096];

void kprintf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    kvsprintf(buf, fmt, ap);

    va_end(ap);

    kwrite(buf);
}

void defer_kprintf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    kvsprintf(buf, fmt, ap);

    va_end(ap);
}

void flush_defer_kprintf() {
    kwrite(buf);
}

void console_init() {
    x = y = 0;
    tab_w = 4;

    // Temporary
    serial_init();
    biosvga_init();

    console_mark_active(0);

    clear();
}

void console_deinit() {
    // Temporary
    biosvga_deinit();
    serial_deinit();
}
